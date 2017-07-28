#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <unistd.h>

#include "client_list.h"
#include "message.pb-c.h"
#include "app_coordinator.h"
#include "server.h"

#define MAX_MSG_SIZE    (1024)

typedef int (*is_alive) (void);

int need_quit (pthread_mutex_t *mtx)
{
    switch (pthread_mutex_trylock (mtx)) {
    case 0: /* if we got the lock, unlock and return 1 (true) */
        pthread_mutex_unlock (mtx);
        return 1;
    case EBUSY: /* return 0 (false) if the mutex was locked */
        return 0;
    }
    return 1;
}

static void *thread_routine (void *args)
{
    client_ctx_t *ctx = (client_ctx_t*) args;
    char in_buff[MAX_MSG_SIZE];
    int sc = 0;
    ServerCommand *cmd = NULL;
    size_t cmd_len;
    mqd_t srv_queue;
    server_message_t msg;

    if (NULL == args) {
        printf ("Thread not started properly.\n");
        return NULL;
    } else {
        printf ("Client connected.\n");
    }

    srv_queue = mq_open(MQ_SRV_NAME, O_WRONLY);
    msg.event_id = EVENT_CLIENT_CONNECTED;
    msg.specific_data = args;
    msg.client_id = ctx->fd;

    sc = mq_send(srv_queue, (char*) &msg, sizeof(server_message_t), 0);

    if (sc < 0) {
        perror ("Couldn't notify connected client.");
        return NULL;
    }

    while (!need_quit(&ctx->mtx)) {
        memset(in_buff, 0, 1024);
        cmd_len = recv (ctx->fd, in_buff, 1024, 0);

        if ((int) cmd_len <= 0) {
             continue;
        }

        cmd = server_command__unpack (NULL, cmd_len, (void*) in_buff);

        if (NULL == cmd) {
            sleep(1);
            continue;
        }

        switch (cmd->command_id) {
            case SERVER_COMMAND__COMMAND__NOP:
                printf("NOP.\n");
                break;
            case SERVER_COMMAND__COMMAND__START:
                printf("Received commdand start.\n");
                break;
            case SERVER_COMMAND__COMMAND__DISCONNECT:
                printf("Client will disconnect soon.\n");
                msg.event_id = EVENT_CLIENT_DISCONNECTED;
                msg.specific_data = args;
                msg.client_id = ctx->fd;
                mq_send(srv_queue, (char*) &msg, sizeof (server_message_t), 0);
                break;
            default:
                printf("Command %d not suported.\n", cmd->command_id);
                break;
        }

        server_command__free_unpacked(cmd, NULL);

    }
    printf("Client %d exites.\n", (int) ctx->fd);
    mq_close (srv_queue);
    return NULL;
}

int start_server (is_alive isalive)
{
    int listenfd = 0;
    int clientfd = 0;
    int sc = 0;
    struct sockaddr_in serv_addr;
    client_ctx_t *cid;

    listenfd = socket (AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (listenfd < 0) {
        perror("Couldn't create socket");
        return -1;
    }

    memset (&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    serv_addr.sin_port = htons (5006);

    sc = bind (listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    if (sc < 0) {
        perror("Server couldn't bind to socker");
        return -1;
    }

    sc = listen (listenfd, 0);

    if (sc < 0) {
        perror ("Can't start listening");
        return -1;
    }

    while (isalive()) {

        clientfd = accept(listenfd, NULL, 0);

        if (clientfd >= 0) {
            cid = malloc (sizeof (client_ctx_t));
            if (NULL == cid) {
                continue;
            }
            cid->fd = clientfd;
            fcntl(cid->fd, F_SETFL, O_NONBLOCK);

            sc += pthread_mutex_init(&cid->mtx, NULL);
            sc += pthread_mutex_lock(&cid->mtx);
            sc += pthread_create(&cid->thread, NULL,
                    thread_routine, (void*) cid);
            if (sc) {
                //TODO: handle errors here
            }
        }
    }

    return 0;
}

void *threaded_server_start (void *args)
{
    is_alive check_func = args;
    int sc = 0;
    if (NULL == args) {
        return NULL;
    }

    sc = start_server(check_func);

    if (0 != sc) {
        perror("Server internal error");
    }

    return NULL;
}
