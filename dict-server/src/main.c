#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

#include "app_coordinator.h"

int main (int argc, char **argv)
{

    pthread_t t;
    int choice = 0;
    mqd_t srv_queue;
    sem_t sem;
    server_message_t msg = {0};

    sem_init(&sem, 0, 0);

    pthread_create(&t, NULL, event_loop, &sem);

    sem_wait(&sem);


    srv_queue = mq_open(MQ_SRV_NAME, O_WRONLY);

    if (srv_queue < 0) {
        perror("Cound't open server pipe");
        //exit(EXIT_FAILURE);
    }

    while (choice != 12) {
        printf("Menu:\n");
        printf("1 - Start the server\n");
        printf("12 - Quit\n");

        fseek(stdin, 0, SEEK_SET);

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                msg.event_id = EVENT_SERVER_START;
                mq_send(srv_queue, (char*) &msg, sizeof (server_message_t), 0);
                break;
            case 12:
                msg.event_id = EVENT_SERVER_STOP;
                mq_send(srv_queue, (char*) &msg, sizeof (server_message_t), 0);
                break;
        }
    }

    mq_close(srv_queue);
    pthread_join(t, NULL);

    return 0;
}



