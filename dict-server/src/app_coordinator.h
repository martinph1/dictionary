#ifndef __APP_COORDINATOR_H__
#define __APP_COORDINATOR_H__

#define MQ_SRV_NAME "/mq_server"
#define MQ_CLT_NAME "client-server"

typedef struct client_message client_message_t;
typedef struct server_message server_message_t;

enum {
    EVENT_SERVER_START,
    EVENT_SERVER_STARTED,
    EVENT_CLIENT_CONNECTED,
    EVENT_CLIENT_DISCONNECTED,
    EVENT_SERVER_STOP,
};

struct client_message {
    int event_id;
    void *specific_data;
};

struct server_message {
    int event_id;
    int client_id;
    void *specific_data;
};

void *event_loop (void *ctx);

#endif /*__APP_COORDINATOR_H__*/
