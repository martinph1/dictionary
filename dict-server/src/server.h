#ifndef __SERVER_H__
#define __SERVER_H__
#include <pthread.h>
#include <stdint.h>

typedef struct client_ctx client_ctx_t;

struct client_ctx {
    int32_t fd;
    pthread_t thread;
    pthread_mutex_t mtx;
    pthread_attr_t attr;
};

void *threaded_server_start (void *args);
#endif /*__SERVER_H__*/
