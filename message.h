#ifndef MESSAGE_H
#define MESSAGE_H

#include "main.h"

typedef enum {
    GARDENER_REQ_TASK = 1,
    GARDENER_ACK_TASK = 2,
    GARDENER_RELEASE_TASK = 3,
    NEW_TASK = 4
} message_type;

typedef struct {
    int ts;       // Lemport's clock
    int src;  

    int type; // message_type - enum is not supported by MPI
    int data;
} message_t;

extern MPI_Datatype MPI_MESSAGE_T;
void inicjuj_typ_pakietu();

#endif
