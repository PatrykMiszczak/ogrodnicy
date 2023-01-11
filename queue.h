#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    int ts; // Lamport timestamp
    int instance_id;

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

void sort_queue(queue_t *queue, int len);

bool check_presence_in_queue(int rank, queue_t *queue);

message_t *get_message(queue_t *queue, int index);

message_t *create_message(int ts, int instance_id);

int compare_messages(const void* m1, const void* m2);

#endif
