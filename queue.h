#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    // TODO: sort by ts and instance_id

    int ts; // timestamp - zegar lamporta

    pthread_mutex_t _mutex;
} message_t;

typedef struct {
    int len;
    int max_len;
    message_t **_q;
    pthread_mutex_t _mutex;
    bool locked;
} queue_t;

void init_queue(queue_t *queue);

void push_message(queue_t *queue, message_t *message);

void remove_message(queue_t *queue, int index);

void lock_queue(queue_t *queue);

void unlock_queue(queue_t *queue);

message_t *get_message(queue_t *queue, int index);

#endif
