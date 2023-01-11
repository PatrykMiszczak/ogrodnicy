#include "test.h"
#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "global_context.h"
#include "message.h"

int rank, size;
state_t stan=InRun;
pthread_t threadKom, threadMon;
pthread_mutex_t stateMut = PTHREAD_MUTEX_INITIALIZER;

#ifndef _TEST

void finalizuj()
{
    pthread_mutex_destroy(&stateMut);
    /* Czekamy, aż wątek potomny się zakończy */
    println("czekam na wątek \"komunikacyjny\"\n" );
    pthread_join(threadKom,NULL);
    MPI_Type_free(&MPI_MESSAGE_T);
    MPI_Finalize();
}

void check_thread_support(int provided)
{
    printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE: 
            printf("Brak wsparcia dla wątków, kończę\n");
            /* Nie ma co, trzeba wychodzić */
	    fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
	    MPI_Finalize();
	    exit(-1);
	    break;
        case MPI_THREAD_FUNNELED: 
            printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
	    break;
        case MPI_THREAD_SERIALIZED: 
            /* Potrzebne zamki wokół wywołań biblioteki MPI */
            printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
	    break;
        case MPI_THREAD_MULTIPLE: printf("Pełne wsparcie dla wątków\n"); /* tego chcemy. Wszystkie inne powodują problemy */
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}

global_context_t *create_global_context()
{
    global_context_t *context = malloc(sizeof(global_context_t));

    context->clock = malloc(sizeof(logic_clock_t));
    context->queue = malloc(sizeof(queue_t));
    context->rank = rank;
    context->size = size;

    logic_clock_init(context->clock);
    init_queue(context->queue);

    return context;
}

void destroy_global_context(global_context_t *context)
{
    free(context->clock);
    free(context->queue);
    free(context);
}

int main(int argc, char **argv)
{
    MPI_Status status;
    int provided;
    message_t pkt;

    global_context_t *context = create_global_context();
    
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    check_thread_support(provided);
    srand(rank);
    inicjuj_typ_pakietu(); // tworzy typ pakietu

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    pthread_create(&threadKom, NULL, startKomWatek , context);

    mainLoop(context);
    
    finalizuj();
    destroy_global_context(context);

    return 0;
}

#endif
