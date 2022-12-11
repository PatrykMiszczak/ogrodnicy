#include "logic_clock.h"

void logic_clock_init(struct logic_clock *clock) {
	pthread_mutex_init(&(clock->_mutex), NULL);
	clock->ts = 0;
}

void logic_clock_update(struct logic_clock *clock, int new_clock) {
	pthread_mutex_lock(&(clock->_mutex));

	if (new_clock > clock) {
		clock->ts = new_clock;
	}

	pthread_mutex_unlock(&(clock->_mutex));
}

int logic_clock_get(struct logic_clock *clock) {
	pthread_mutex_lock(&(clock->_mutex));

	int ret = clock->ts;

	pthread_mutex_unlock(&(clock->_mutex));

	return ret;
}
