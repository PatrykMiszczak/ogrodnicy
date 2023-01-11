#include "main.h"
#include "message.h"

#define NITEMS 4

MPI_Datatype MPI_MESSAGE_T;

/* tworzy typ MPI_MESSAGE_T
*/
void inicjuj_typ_pakietu()
{
    /* Stworzenie typu */
    /* Poniższe (aż do MPI_Type_commit) potrzebne tylko, jeżeli
       brzydzimy się czymś w rodzaju MPI_Send(&typ, sizeof(pakiet_t), MPI_BYTE....
    */
    /* sklejone z stackoverflow */
    int       blocklengths[NITEMS] = {1,1,1};
    MPI_Datatype typy[NITEMS] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[NITEMS]; 
    offsets[0] = offsetof(message_t, ts);
    offsets[1] = offsetof(message_t, src);
    offsets[2] = offsetof(message_t, type);
    offsets[3] = offsetof(message_t, data);

    MPI_Type_create_struct(NITEMS, blocklengths, offsets, typy, &MPI_MESSAGE_T);

    MPI_Type_commit(&MPI_MESSAGE_T);
}
