#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "queue.h"

#ifdef _TEST

message_t *create_message(int ts);

void dump(queue_t* queue) {
    printf("## dump\n");

    for (int i = 0; i < queue->len; i++) {
        printf("%d\n", get_message(queue, i)->ts);
    }
}

void _queueAddTest() {
    printf("# Queue add TEST\n");

    queue_t queue;

    init_queue(&queue);

    push_message(&queue, create_message(1));
    push_message(&queue, create_message(2));
    push_message(&queue, create_message(10));
    push_message(&queue, create_message(3));
    push_message(&queue, create_message(7));
    push_message(&queue, create_message(5));
    push_message(&queue, create_message(6));
    push_message(&queue, create_message(4));
    push_message(&queue, create_message(9));
    push_message(&queue, create_message(8));

    // Expected output: 1 .. 10
    dump(&queue);
}

void _queueRemoveTest() {
    printf("# Queue remove TEST\n");

    queue_t queue;

    init_queue(&queue);

    push_message(&queue, create_message(1));
    push_message(&queue, create_message(2));
    push_message(&queue, create_message(3));
    push_message(&queue, create_message(4));
    push_message(&queue, create_message(5));

    remove_message(&queue, 2);
    remove_message(&queue, 0);
    remove_message(&queue, 2);

    // Expected output: 2, 4
    dump(&queue);
}

void _queueRemoveAddTest() {
    printf("# Queue remove & add TEST\n");

    queue_t queue;

    init_queue(&queue);

    push_message(&queue, create_message(1));
    push_message(&queue, create_message(2));
    push_message(&queue, create_message(3));
    push_message(&queue, create_message(4));
    push_message(&queue, create_message(5));

    remove_message(&queue, 2);
    remove_message(&queue, 0);
    remove_message(&queue, 2);
    
    push_message(&queue, create_message(0));
    push_message(&queue, create_message(10));
    push_message(&queue, create_message(3));

    // Expected output: 0, 2, 3, 4, 10
    dump(&queue);
}

int main() {
    _queueAddTest();
    _queueRemoveTest();
    _queueRemoveAddTest();
}

message_t *create_message(int ts) {
    message_t *message = malloc(sizeof(message_t));

    message->ts = ts;

    return message;
}

#endif
