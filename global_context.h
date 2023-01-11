#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

#include "logic_clock.h"
#include "queue.h"

typedef struct {
	int rank;
	int size;
	logic_clock_t *clock;
	queue_t *queue;
} global_context_t;

#endif // THREAD_CONTEXT_H
