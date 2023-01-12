#include "main.h"
#include "watek_glowny.h"
#include "util.h"

void mainLoop(global_context_t *context)
{
	logic_clock_t *clock = context->clock;
	queue_t *queue_gardeners = context->queue_gardeners;
	queue_t *queue_tasks= context->queue_tasks;

	srandom(rank);
	sleep(rand() % 3);
	message_type message_type;
	message_tag tag;

	while (stan != InFinish) {
		// Instytut
		if (rank == ROOT) {

			debug("Zmieniam stan na wysyłanie o nowej pracy");

			changeState(InSend);
			for (int i = 0; i < 4; i++)
			{
				message_t *message = malloc(sizeof(message_t));
				message->type = NEW_TASK;
				tag = AppPkt;

				broadcastMessage(context, message, tag);
			}
			
			sleep(3);
			changeState(InRun);

			// debug("Skończyłem wysyłać o nowej pracy");
		}
		// Ogrodnik
		else {
			if (stan == InRun) {

				lock_queue(queue_gardeners);
				bool present = check_presence_in_queue(rank, queue_gardeners);
				unlock_queue(queue_gardeners);		

				if(!present){
					// debug("Nie ma mnie w kolejce wysyłam zgłoszenie do gotowości.");

					changeState(InSend);
					message_t *message = malloc(sizeof(message_t));
					message->type = GARDENER_REQ_TASK;
					tag = AppPkt;

					broadcastMessage(context, message, tag);

					changeState(InRun);

					// debug("Skończyłem wysyłać");
				}

				// TODO: `context->queue_tasks->len != 0` should be in lock

				if(context->queue_tasks->len != 0 && canProcessTask(context)){
					changeState(InReadingLiterature);
					lock_queue(context->queue_tasks);
					debug("get message watekglowny L65")
					debug("Gardener: %d podejmuje zadanie %d", context->rank, get_message(context->queue_tasks, 0)->ts);
					unlock_queue(context->queue_tasks);
					message_t *message_rel = malloc(sizeof(message_t));
					message_rel->type = GARDENER_RELEASE_TASK;
					tag = AppPkt;
					broadcastMessage(context, message_rel, tag);
					free(message_rel);
				}

				// debug("## dump queue_gardeners\n");
				// lock_queue(queue_gardeners);
				// for (int i = 0; i < queue_gardeners->len; i++) {
				// 	debug("%d %d\n", get_message(queue_gardeners, i)->ts, get_message(queue_gardeners, i)->src);
				// }
				// unlock_queue(queue_gardeners);

				// debug("## dump queue_tasks\n");
				// lock_queue(queue_tasks);
				// for (int i = 0; i < queue_tasks->len; i++) {
				// 	debug("%d %d\n", get_message(queue_tasks, i)->ts, get_message(queue_tasks, i)->src);
				// }
				// unlock_queue(queue_tasks);

			} else if (stan == InReadingLiterature){
				// implement sleeping random time and changing state to collecting stuff
				int sleep_time = (rand() % 3) + 1;

				// debug("Podejmuję nowe zadanie i idę czytać na %d sekund", sleep_time)

				sleep(sleep_time);
				changeState(InCollectingStuff);
				pthread_mutex_lock(&(context->agreement_num_mutex));
				context->agreement_num = 0;
				pthread_mutex_unlock(&(context->agreement_num_mutex));
				message_t *message = malloc(sizeof(message_t));
				message->type = GARDENER_REQ_TOOL;
				tag = AppPkt;

				broadcastMessage(context, message, tag);

			} else if (stan == InCollectingStuff) {
				// collectingstuff

				int tool_amount = 2; // TODO: make global or in context

				debug("proc: %d agreement_num: %d", context->rank, context->agreement_num);

				if (context->agreement_num < context->size - tool_amount)
				{
					changeState(InWorkingOnTask);
				}
				
			} else if (stan == InWorkingOnTask) {
				int sleep_time = (rand() % 3) + 1;
				sleep(sleep_time);
				changeState(InRun);

			}
		}

		sleep(SEC_IN_STATE);
	}
}
