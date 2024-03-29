#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

#include "logic_clock.h"
#include "queue.h"

typedef struct {
	int rank;
	int size;
	logic_clock_t *clock;
	queue_t *queue_gardeners;
	queue_t *queue_tasks;
	bool *gardeners_waiting_for_tool;
	int agreement_num;
	pthread_mutex_t agreement_num_mutex;
	int *gardeners_clocks;
	pthread_mutex_t gardeners_clocks_mutex;
	int tool_request_ts;
} global_context_t;

#endif // THREAD_CONTEXT_H
