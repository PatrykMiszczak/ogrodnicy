#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(thread_context_t *context)
{
    logic_clock_t *clock = context->clock;
    queue_t *queue = context->queue;

    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;

    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while (stan != InFinish) {
        debug("czekam na recv");

        MPI_Recv(&pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        logic_clock_update(clock, pakiet.ts);

        switch (status.MPI_TAG) {
            case Finish: 
                    changeState(InFinish);
            break;

            case AppPkt: 
                    debug("Dostałem pakiet od %d z danymi %d (ts = %d)", pakiet.src, pakiet.data, pakiet.ts);

                    // TODO: push to queue
            break;

            default:
            break;
        }
    }

    return NULL;
}
