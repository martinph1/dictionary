#include <stdlib.h>
#include <stdint.h>

#include "client_list.h"

struct list_ptv_ctx {
    client_node_t *head;
    size_t size;
};

void *client_list_init ()
{
    struct list_ptv_ctx *ctx;

    ctx = malloc(sizeof (struct list_ptv_ctx));

    ctx->head = NULL;
    ctx->size = 0;

    return (void*) ctx;
}

int client_list_add (void *list_hnd, void *clinet_priv, uint32_t id)
{
    struct list_ptv_ctx *ctx = (struct list_ptv_ctx*) list_hnd;

    client_node_t *client = malloc (sizeof (client_node_t));

    if (NULL == ctx || NULL == client) {
        return -1;
    }

    client->priv = clinet_priv;
    client->next = ctx->head;
    client->id = id;
    ctx->head = client;
    ctx->size++;

    return 0;
}

void *client_list_pop (void *list_hnd)
{
    struct list_ptv_ctx *ctx = (struct list_ptv_ctx*) list_hnd;
    void *res = NULL;
    client_node_t *temp = NULL;

    if (NULL == ctx || NULL == ctx->head) {
        return NULL;
    }

    res = ctx->head->priv;
    temp = ctx->head;
    ctx->head = ctx->head->next;
    ctx->size--;
    free (temp);

    return res;
}

int client_list_destroy (void *list_hnd)
{
    struct list_ptv_ctx *ctx = (struct list_ptv_ctx*) list_hnd;

    if (NULL == ctx) {
        return -1;
    }

    while (ctx->head != NULL) {
        client_list_pop(list_hnd);
    }

    if (0 == ctx->size) {
        free (list_hnd);
        return 0;
    } else {
        return -1;
    }

}


void *client_list_pop_id (void *list_hnd, uint32_t id)
{
    struct list_ptv_ctx *ctx = (struct list_ptv_ctx*) list_hnd;
    void *res = NULL;
    client_node_t *iter = NULL;
    client_node_t *prev_ptr = NULL;

    if (NULL == ctx) {
        return NULL;
    }

    if (id == ctx->head->id) {
        return client_list_pop(list_hnd);
    } else {
        iter = ctx->head->next;
        prev_ptr = ctx->head;
    }

    for ( ;iter != NULL; iter = iter->next,
                         prev_ptr = prev_ptr->next) {
        if (iter->id == id) {
            res = iter->priv;
            prev_ptr->next = iter->next;
            free(iter);
            return res;
        }

    }

    return NULL;

}
