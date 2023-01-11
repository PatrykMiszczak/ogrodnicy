
#include <assert.h>
#include <stdlib.h>

#include "queue.h"

void init_queue(queue_t *queue) {
    queue->len = 0;
    queue->max_len = 0;
    queue->_q = NULL;
    pthread_mutex_init(&(queue->_mutex), NULL);
    queue->locked = false;
}

void push_message(queue_t *queue, message_t *message) {
    assert(queue->locked);
    assert(queue->len <= queue->max_len);

    if (queue->max_len == 0) {
        queue->max_len = 1;
        queue->len = 1;

        queue->_q = malloc(sizeof(message_t *));
        queue->_q[0] = message;
        
        return;
    }
        
    if (queue->len == queue->max_len) {
        queue->max_len++;

        queue->_q = realloc(queue->_q, queue->max_len * sizeof(message_t *));
    }

    queue->len++;
    
    int i;

    for (i = queue->len - 2; i >= 0; i--) {
        if (queue->_q[i]->ts < message->ts) {
            break;
        }
    }

    i = i+1;

    for (int j = queue->len - 2; j >= i; j--) {
        queue->_q[j+1] = queue->_q[j];
    }

    queue->_q[i] = message;
}

void remove_message(queue_t *queue, int index) {
    assert(queue->locked);
    assert(index >= 0);
    assert(index < queue->len);

    free(queue->_q[index]);

    for (int i = index; i < queue->len-1; i++) {
        queue->_q[i] = queue->_q[i+1];
    }

    queue->_q[queue->len-1] = NULL;

    queue->len--;
}

message_t *get_message(queue_t *queue, int index) {
    assert(queue->locked);
    assert(index >= 0);
    assert(index < queue->len);

    return queue->_q[index];
}

message_t *create_message(int ts, int instance_id) {
    message_t *message = malloc(sizeof(message_t));

    message->ts = ts;
    message->instance_id = instance_id;

    return message;
}

int compare_messages(const void *m1, const void *m2){
    const message_t *tmp1 = (message_t *)m1;
    const message_t *tmp2 = (message_t *)m2;
    if (tmp1->ts < tmp2->ts)
        return -1;
    else if (tmp1->ts > tmp2->ts)
        return +1;
    else
        if (tmp1->instance_id < tmp2->instance_id)
            return -1;
        else if (tmp1->instance_id > tmp2->instance_id)
            return +1;
        else
            return 0;
}

void sort_queue(queue_t *queue, int len){
    assert(queue->locked);
    qsort(queue, len, sizeof(message_t), compare_messages);
}

bool check_presence_in_queue(int rank, queue_t *queue){
    assert(queue->locked);
    assert(rank >= 0);
    for(int i = 0; i < queue->len; i++){
        if (get_message(queue, i)->instance_id == rank) 
            return true;
    }
    return false;
}

void lock_queue(queue_t *queue) {
    pthread_mutex_lock(&(queue->_mutex));
    queue->locked = true;
}

void unlock_queue(queue_t *queue) {
    queue->locked = false;
    pthread_mutex_unlock(&(queue->_mutex));
}

