#ifndef __CLIENT_LIST_H__
#define __CLIENT_LIST_H__

#include <stdint.h>

typedef struct client_node client_node_t;

struct client_node {
    client_node_t *next;
    uint32_t id;
    void *priv;
};

void *client_list_init ();
int client_list_add (void *list_hnd, void *clinet_priv, uint32_t id);
void *client_list_pop (void *list_hnd);
int client_list_destroy (void *list_hnd);
void *client_list_pop_id (void *list_hnd, uint32_t id);

#endif /* __CLIENT_LIST_H__ */
