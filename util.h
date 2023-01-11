#ifndef UTILH
#define UTILH

#include "main.h"
#include "logic_clock.h"

typedef enum {
    ReadyForNewTaskMessage = 1,
    NewTaskMessage = 2
} message_type;

/* typ pakietu */
typedef struct {
    int ts;       // Lemport's clock
    int src;  
    message_type data;     //informs about type of message (gardener is waiting for new task, finished task)
} packet_t;

/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 3

typedef enum {
    AppPkt = 1,
    Finish = 2
} packet_tag;

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), z jakim typem */
void sendPacket(logic_clock_t * clock, packet_t *pkt, packet_tag tag); //zawsze do wszystkich wysyłamy, w tym również do samego siebie

#endif
