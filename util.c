#include <assert.h>

#include "main.h"
#include "util.h"
#include "global_context.h"
#include "message.h"

struct tagNames_t{
    const char *name;
    int tag;
} tagNames[] = {{ "pakiet aplikacyjny", AppPkt }, { "finish", Finish }};

const char *tag2string(message_tag tag)
{
    int tagsLen = sizeof(tagNames) / sizeof(struct tagNames_t);

    for (int i=0; i < tagsLen; i++) {
        if ( tagNames[i].tag == tag )
            return tagNames[i].name;
    }

    return "<unknown>";
}

int broadcastMessage(global_context_t *context, message_t *pkt, message_tag tag)
{
    int ts = logic_clock_get(context->clock);

    pkt->ts = ts;
    // wysyłamy do wszystkich oraz instytutu (o rankingu 0)
    for (int i = 1; i < size; i++) {
        _sendMessage(pkt, i, tag);
    }

    free(pkt);

    return ts;
}

int sendMessage(message_t *pkt, int destination, message_tag tag) {
    int ts = _sendMessage(pkt, destination, tag, true);

    free(pkt);

    return ts;
}

/* opis patrz util.h */
int _sendMessage(message_t *pkt, int destination, message_tag tag)
{
    assert(pkt != NULL);

    if (pkt->ts == 0) {
        pkt->ts = logic_clock_get(clock);
    }

    pkt->src = rank;

    MPI_Send(pkt, 1, MPI_MESSAGE_T, destination, tag, MPI_COMM_WORLD);

    // debug("Wysyłam %s do %d (tc = %d) o tresci: %d\n", tag2string(tag), destination, pkt->ts, pkt->data);

    return pkt->ts;
}

void changeState( state_t newState )
{
    pthread_mutex_lock( &stateMut );

    if (stan==InFinish) { 
        pthread_mutex_unlock( &stateMut );

        return;
    }

    stan = newState;

    pthread_mutex_unlock( &stateMut );
}

bool canProcessTask(global_context_t *context) {
    queue_t *queue_gardeners = context->queue_gardeners;
    int *gardeners_clocks = context->gardeners_clocks;
    message_t *message;

    lock_queue(queue_gardeners);
    if (context->queue_gardeners->len == 0){
        unlock_queue(queue_gardeners);
        return false;
    }
    message = get_message(queue_gardeners, 0);
    unlock_queue(queue_gardeners);

    // debug("Top message source %d. My rank %d.", message->src, context->rank);

    if (message->src != context->rank) {
        return false;
    }

    pthread_mutex_lock(&(context->gardeners_clocks_mutex));

    int self_clock = gardeners_clocks[context->rank];

    // debug("Self clock %d", self_clock);

    // 1 - skip institute
    for (int i = 1; i < context->size; i++) {
        // debug("gardener %d clock = %d", i, gardeners_clocks[i]);

        if (gardeners_clocks[i] == 0 || gardeners_clocks[i] < self_clock) {
            pthread_mutex_unlock(&(context->gardeners_clocks_mutex));
            return false;
        }
    }

    pthread_mutex_unlock(&(context->gardeners_clocks_mutex));

    return true;
}

bool hasHigherPriority(int ts_1, int rank_1, int ts_2, int rank_2) {
    if (ts_1 < ts_2)
        return true;
    
    if (ts_1 == ts_2 && rank_1 < rank_2)
        return true;

    return false;
}
