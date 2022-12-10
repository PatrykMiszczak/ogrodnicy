
#include <assert.h>
#include <stdlib.h>

#include "queue.h"

void init_queue(queue_t *queue) {
    queue->len = 0;
    queue->max_len = 0;
    queue->_q = NULL;
}

void push_message(queue_t *queue, message_t *message) {
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
    assert(index >= 0);
    assert(index < queue->len);

    return queue->_q[index];
}
