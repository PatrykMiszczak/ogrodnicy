#ifndef MESSAGE_H
#define MESSAGE_H

#include "main.h"

typedef struct {
    int ts;       // Lemport's clock
    int src;  

    int data;
} message_t;

extern MPI_Datatype MPI_MESSAGE_T;
void inicjuj_typ_pakietu();

#endif
