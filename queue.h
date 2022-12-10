#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    // TODO: sort by ts and instance_id

    int ts; // timestamp - zegar lamporta

    
} message_t;

typedef struct {
    int len;
    int max_len;
    message_t **_q;
} queue_t;

void init_queue(queue_t *queue);

void push_message(queue_t *queue, message_t *message);

void remove_message(queue_t *queue, int index);

message_t *get_message(queue_t *queue, int index);

#endif
