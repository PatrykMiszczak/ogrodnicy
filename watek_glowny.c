#include "main.h"
#include "watek_glowny.h"

void mainLoop(thread_context_t *context)
{
	logic_clock_t *clock = context->clock;
	queue_t *queue = context->queue;

	srandom(rank);
	sleep(rand()%3);
	message_type message_type;
	packet_tag tag;

	while (stan != InFinish) {

		if (rank == ROOT){ //instytut
			sleep(10);
			debug("Zmieniam stan na wysyłanie o nowej pracy");

			changeState(InSend);

			packet_t *pkt = malloc(sizeof(packet_t));
			pkt->data = NewTaskMessage;
			tag = AppPkt;
			
			sendPacket(clock, pkt, tag);

			changeState(InRun);

			debug("Skończyłem wysyłać o nowej pracy");

			
		} else { //ogrodnik
			if (stan == InReadingLiterature){
				//implement sleeping random time and changing state to collecting stuff
				int sleep_time = (rand() % 3) + 1;
				debug("Podejmuję nowe zadanie i idę czytać na %d sekund", sleep_time)
				sleep(sleep_time);
				changeState(InCollectingStuff);

			} else if (stan == InCollectingStuff) {
				//collectingstuff

				sleep(1);

				changeState(InWorkingOnTask);
			} else if (stan == InWorkingOnTask) {
				sleep(1);
				changeState(InSend);
				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->data = ReadyForNewTaskMessage;

				tag = AppPkt;
				
				sendPacket(clock, pkt, tag);

				changeState(InRun);

				debug("Skończyłem wysyłać");

			} else if (stan == InRun) {

				lock_queue(queue);
				bool present = check_presence_in_queue(rank, queue);
				unlock_queue(queue);		

				if(!present){
					debug("Nie ma mnie w kolejce wysyłam zgłoszenie do gotowości.");
					changeState(InSend);
					packet_t *pkt = malloc(sizeof(packet_t));
					pkt->data = ReadyForNewTaskMessage;

					tag = AppPkt;
					
					sendPacket(clock, pkt, tag);

					changeState(InRun);

					debug("Skończyłem wysyłać");
				}
				debug("## dump\n");
				lock_queue(queue);
				for (int i = 0; i < queue->len; i++) {
					debug("%d %d\n", get_message(queue, i)->ts, get_message(queue, i)->instance_id);
				}
				unlock_queue(queue);
			} else {
			}
		}

		sleep(SEC_IN_STATE);
	}
}
