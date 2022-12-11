#ifndef LOGIC_CLOCK_H
#define LOGIC_CLOCK_H

#include <pthread.h>

typedef struct {
	int ts;	// Timestamp

	pthread_mutex_t _mutex;
} logic_clock_t;

void logic_clock_create(logic_clock_t *clock);

/**
 * @brief Set the current time of the logic clock if it is greater than the current time.
 */
void logic_clock_update(logic_clock_t *clock, int timestamp);

/**
 * @brief Get the current time of the logic clock and increment it.
 */
int logic_clock_get(logic_clock_t *clock);

#endif // LOGIC_CLOCK_H
