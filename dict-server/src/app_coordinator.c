#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

#include "app_coordinator.h"
#include "server.h"
#include "client_list.h"

int app_coord_alive_flag = 0;

int check_alive ()
{
    return app_coord_alive_flag;
}

void *event_loop (void *ctx)
{
    struct mq_attr attr;
    void *client_list = NULL;
    mqd_t srv_mq;
    client_ctx_t *cl_ctx;
    server_message_t srv_msg;
    pthread_t srv_th;

    printf ("Initializing event loop.\n");

    attr.mq_maxmsg = 300;
    attr.mq_msgsize = sizeof(server_message_t);
    attr.mq_flags = 0;

    srv_mq = mq_open (MQ_SRV_NAME, O_RDONLY | O_CREAT, 0664, &attr);
    mq_getattr (srv_mq, &attr);

    while ((int) attr.mq_curmsgs) {
        mq_receive (srv_mq, (char*) &srv_msg, sizeof(server_message_t), 0);
        mq_getattr (srv_mq, &attr);
    }

    client_list = client_list_init ();

    sem_post ((sem_t*) ctx);

    if (NULL == client_list) {
        fprintf (stderr, "Could not allocate client list.\n");
        return NULL;
    }

    while (1) {
        mq_receive (srv_mq, (char*) &srv_msg, sizeof(server_message_t), NULL);

        switch (srv_msg.event_id) {
        case EVENT_SERVER_START:
            printf ("EVENT_SERVER_START\n");
            if (1 == app_coord_alive_flag) {
                printf ("Skipping server start due to already running\n");
            } else {
                printf ("Starting service\n");
                app_coord_alive_flag = 1;
                pthread_create (&srv_th, NULL, threaded_server_start,
                        check_alive);
            }
            break;
        case EVENT_CLIENT_CONNECTED:
            printf ("EVENT_CLIENT_CONNECTED\n");
            client_list_add (client_list, srv_msg.specific_data,
                    srv_msg.client_id);
            printf ("Connected client with id: %d\n", srv_msg.client_id);
            break;
        case EVENT_CLIENT_DISCONNECTED:
            printf ("EVENT_CLIENT_DISCONNECTED\n");
            cl_ctx = client_list_pop_id (client_list, srv_msg.client_id);
            if (NULL != cl_ctx) {
                pthread_mutex_unlock (&cl_ctx->mtx);
                pthread_join (cl_ctx->thread, NULL);
                free (cl_ctx);
                printf ("Client %d cleaned.\n", srv_msg.client_id);
            }
            break;
        case EVENT_SERVER_STOP:
            printf ("EVENT_SERVER_STOP\n");
            printf ("Stopping server\n");

            cl_ctx = client_list_pop (client_list);
            while (cl_ctx) {
                pthread_mutex_unlock (&cl_ctx->mtx);
                pthread_join (cl_ctx->thread, NULL);
                free (cl_ctx);
                cl_ctx = client_list_pop (client_list);
            }
            if (app_coord_alive_flag) {
                app_coord_alive_flag = 0;
                pthread_join (srv_th, NULL);
            }
            client_list_destroy (client_list);
            mq_unlink (MQ_SRV_NAME);
            mq_close (srv_mq);
            return NULL;
            break;
        default:
            break;
        }
    }

}
