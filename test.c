#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "queue.h"

#ifdef _TEST

message_t *create_message(int ts);

#define assert(x) if (!(x)) { printf("Assertion failed: %s (%s:%d) \n", #x, __FILE__, __LINE__); }

void dump(queue_t* queue) {
    printf("## dump\n");

    for (int i = 0; i < queue->len; i++) {
        printf("%d\n", get_message(queue, i)->ts);
    }
}

void _queueAdd_test() {
    printf("# Queue add TEST\n");

    queue_t queue;

    init_queue(&queue);
    lock_queue(&queue);

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

    assert(queue.len == 10);
    assert(get_message(&queue, 0)->ts == 1);
    assert(get_message(&queue, 1)->ts == 2);
    assert(get_message(&queue, 2)->ts == 3);
    assert(get_message(&queue, 3)->ts == 4);
    assert(get_message(&queue, 4)->ts == 5);
    assert(get_message(&queue, 5)->ts == 6);
    assert(get_message(&queue, 6)->ts == 7);
    assert(get_message(&queue, 7)->ts == 8);
    assert(get_message(&queue, 8)->ts == 9);
    assert(get_message(&queue, 9)->ts == 10);

    unlock_queue(&queue);
}

void _queueRemove_test() {
    printf("# Queue remove TEST\n");

    queue_t queue;

    init_queue(&queue);
    lock_queue(&queue);

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

    assert(queue.len == 2);
    assert(get_message(&queue, 0)->ts == 2);
    assert(get_message(&queue, 1)->ts == 4);

    unlock_queue(&queue);
}

void _queueRemoveAdd_test() {
    printf("# Queue remove & add TEST\n");

    queue_t queue;

    init_queue(&queue);
    lock_queue(&queue);

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

    assert(queue.len == 5);
    assert(get_message(&queue, 0)->ts == 0);
    assert(get_message(&queue, 1)->ts == 2);
    assert(get_message(&queue, 2)->ts == 3);
    assert(get_message(&queue, 3)->ts == 4);
    assert(get_message(&queue, 4)->ts == 10);

    unlock_queue(&queue);
}

int main() {
    _queueAdd_test();
    _queueRemove_test();
    _queueRemoveAdd_test();
}

message_t *create_message(int ts) {
    message_t *message = malloc(sizeof(message_t));

    message->ts = ts;

    return message;
}

#endif
