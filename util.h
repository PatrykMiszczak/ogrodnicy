#ifndef UTILH
#define UTILH

#include "main.h"
#include "logic_clock.h"

/* typ pakietu */
typedef struct {
    int ts;       // Lemport's clock
    int src;  

    int data;
} packet_t;

/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */
#define NITEMS 3

typedef enum {
    AppPkt = 1,
    Finish = 2
} packet_tag;

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(logic_clock_t * clock, packet_t *pkt, int destination, packet_tag tag);

#endif
