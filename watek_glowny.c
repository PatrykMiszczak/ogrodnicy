#include "main.h"
#include "watek_glowny.h"

void mainLoop(thread_context_t *context)
{
	logic_clock_t *clock = context->clock;
	queue_t *queue = context->queue;

	srandom(rank);

	packet_tag tag;

	while (stan != InFinish) {
		int perc = random() % 100; 

		if (perc < STATE_CHANGE_PROB) {
			if (stan==InRun) {
				debug("Zmieniam stan na wysyłanie");

				changeState(InSend);

				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->data = perc;
				perc = random() % 100;
				tag = ( perc < 25 ) ? Finish : AppPkt;

				debug("Perc: %d", perc);
				
				sendPacket(clock, pkt, (rank + 1) % size, tag);

				changeState(InRun);

				debug("Skończyłem wysyłać");
			} else {
			}
		}

		sleep(SEC_IN_STATE);
	}
}
