#include "main.h"
#include "watek_komunikacyjny.h"
#include "util.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(global_context_t *context)
{
    logic_clock_t *clock = context->clock;
    queue_t *queue_gardeners = context->queue_gardeners;
    queue_t *queue_tasks = context->queue_tasks;

    MPI_Status status;
    int is_message = FALSE;
    message_t pakiet;

    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while (stan != InFinish) {
        // debug("czekam na recv");

        MPI_Recv(&pakiet, 1, MPI_MESSAGE_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // debug("Dostałem pakiet od %d z danymi %d, message typpe: %d (ts = %d)", pakiet.src, pakiet.data, pakiet.type, pakiet.ts);


        logic_clock_update(clock, pakiet.ts);

        pthread_mutex_lock(&(context->gardeners_clocks_mutex));
        context->gardeners_clocks[pakiet.src] = pakiet.ts;
        pthread_mutex_unlock(&(context->gardeners_clocks_mutex));

        switch (status.MPI_TAG) {
            case Finish: 
                    changeState(InFinish);
            break;

            case AppPkt:


                    switch (pakiet.type){ //checking the packet message type

                        case GARDENER_REQ_TASK:
                            // debug("Dodaje %d czekającego do kolejki", pakiet.src);
                            lock_queue(queue_gardeners);

                            message_t *msg_gardener = malloc(sizeof(message_t));
                            *msg_gardener = pakiet;

                            push_message(queue_gardeners, msg_gardener);

                            message_t *message_req = malloc(sizeof(message_t));
                            message_req->type = GARDENER_ACK_TASK;
                            message_tag tag = AppPkt;
                            broadcastMessage(context, message_req, tag);
                            free(message_req);

                            unlock_queue(queue_gardeners);
                        break;

                        case GARDENER_ACK_TASK:
                            break;

                        case GARDENER_RELEASE_TASK:
                            lock_queue(queue_gardeners);
                            remove_message(queue_gardeners, 0);
                            unlock_queue(queue_gardeners);

                            lock_queue(queue_tasks);
                            remove_message(queue_tasks, 0);
                            unlock_queue(queue_tasks);
                        break;

                        case GARDENER_ACK_TOOL:
                            pthread_mutex_lock(&(context->agreement_num_mutex));
                            context->agreement_num++;
                            pthread_mutex_unlock(&(context->agreement_num_mutex));
                        break;

                        case GARDENER_REQ_TOOL:
                            if (pakiet.src == rank)
                                break;

                            // TODO: if waiting for tool from gardener and the priority is lower, add to queue
                            if (stan == InWorkingOnTask) {
                                context->gardeners_waiting_for_tool[pakiet.src] = true;

                                break;
                            }

                            message_t *message = malloc(sizeof(message_t));
                            message->type = GARDENER_ACK_TOOL;
                            message->ts = 0;
                            tag = AppPkt;

                            sendMessage(context, message, pakiet.src, tag);
                        break;

                        case NEW_TASK:
                            lock_queue(queue_tasks);

                            message_t *msg_task = malloc(sizeof(message_t));
                            *msg_task = pakiet;

                            push_message(queue_tasks, msg_task);
                            unlock_queue(queue_tasks);
                        break;

                        default:
                            debug("Something went wrong: pakiet.type");
                            break;
                    }
            break;

            default:
                debug("Something went wrong: status.MPI_TAG")
            break;
        }
    }

    return NULL;
}
