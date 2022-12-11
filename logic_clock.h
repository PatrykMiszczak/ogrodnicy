#ifndef LOGIC_CLOCK_H
#define LOGIC_CLOCK_H

#include <pthread.h>

struct logic_clock {
	int ts;	// Timestamp

	pthread_mutex_t _mutex;
};

void logic_clock_init(struct logic_clock *clock);

/**
 * @brief Set the current time of the logic clock if it is greater than the current time.
 */
void logic_clock_update(struct logic_clock *clock, int new_time);

/**
 * @brief Get the current time of the logic clock and increment it.
 */
int logic_clock_get(struct logic_clock *clock);

#endif // LOGIC_CLOCK_H
