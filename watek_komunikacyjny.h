#ifndef WATEK_KOMUNIKACYJNY_H
#define WATEK_KOMUNIKACYJNY_H

#include "thread_context.h"

/* wątek komunikacyjny: odbieranie wiadomości i reagowanie na nie poprzez zmiany stanu */
void *startKomWatek(thread_context_t *context);

#endif
