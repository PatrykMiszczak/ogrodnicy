#include "logic_clock.h"

void logic_clock_init(logic_clock_t *clock) {
	pthread_mutex_init(&(clock->_mutex), NULL);
	clock->ts = 0;
}

void logic_clock_update(logic_clock_t *clock, int timestamp) {
	pthread_mutex_lock(&(clock->_mutex));

	if (timestamp > clock->ts) {
		clock->ts = timestamp;
	}

	pthread_mutex_unlock(&(clock->_mutex));
}

int logic_clock_get(logic_clock_t *clock) {
	pthread_mutex_lock(&(clock->_mutex));

	// TODO: Should be before or after?
	clock->ts++;

	int ret = clock->ts;

	pthread_mutex_unlock(&(clock->_mutex));

	return ret;
}
