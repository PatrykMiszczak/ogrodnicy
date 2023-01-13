#ifndef UTILH
#define UTILH

#include "logic_clock.h"
#include "global_context.h"
#include "message.h"

typedef enum {
    AppPkt = 1,
    Finish = 2
} message_tag;

int broadcastMessage(global_context_t *context, message_t *pkt, message_tag tag);

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
int sendMessage(message_t *pkt, int destination, message_tag tag);

/* Sprawdzenie warunków wejścia do sekcji krytycznej w algorytmie Lamporta */
bool canProcessTask(global_context_t *context);

bool hasHigherPriority(int ts_1, int rank_1, int ts_2, int rank_2);

#endif
