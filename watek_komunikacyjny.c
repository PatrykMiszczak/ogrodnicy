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
            switch (pakiet.data){ //checking the packet message type
            
                case ReadyForNewTaskMessage:
                    debug("Dostałem pakiet od %d z danymi %d (ts = %d). Dodaje czekającego do kolejki", pakiet.src, pakiet.data, pakiet.ts);
                    lock_queue(queue);
                    push_message(queue, create_message(pakiet.ts, pakiet.src));
                    sort_queue(queue, queue->len);
                    unlock_queue(queue);
                break;
                
                case NewTaskMessage:
                    lock_queue(queue);
                    if(rank == get_message(queue, 0)->instance_id)
                        changeState(InReadingLiterature);

                    remove_message(queue, 0);
                    unlock_queue(queue);
                break;

                default:
                    break;
            }
            break;

            default:
            break;
        }
    }

    return NULL;
}
