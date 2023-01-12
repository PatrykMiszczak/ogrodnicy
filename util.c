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

void broadcastMessage(global_context_t *context, message_t *pkt, message_tag tag)
{
    pkt->ts = logic_clock_get(context->clock);
    // wysyłamy do wszystkich oraz instytutu (o rankingu 0)
    for (int i = 1; i < size; i++) {
        sendMessage(context->clock, pkt, i, tag);
    }
}

/* opis patrz util.h */
void sendMessage(logic_clock_t *clock, message_t *pkt, int destination, message_tag tag) //TODO: jeśli zegar nie jest ustawiony, ustaw w send message
{
    int freepkt = 0;

    if (pkt == 0) {
        pkt = malloc(sizeof(message_t));

        freepkt = 1;
    }

    pkt->src = rank;

    MPI_Send(pkt, 1, MPI_MESSAGE_T, destination, tag, MPI_COMM_WORLD);

    debug("Wysyłam %s do %d (tc = %d) o tresci: %d\n", tag2string(tag), destination, pkt->ts, pkt->data);

    if (freepkt) {
        free(pkt);
    }
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
