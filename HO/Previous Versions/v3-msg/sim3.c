#include "./sim.h"

#define WIDTH 10
#define DEPTH 10

extern int num_delt_ints;
extern int num_delt_exts;
extern int num_event_exts;
extern int atomics;

xbt_mutex_t m;

int atomic(int argc, char *argv[])
{
	char* hostN[30], dest[30];
	sprintf(hostN, "%s", argv[2]);
	int depth = atoi(argv[0]), element = atoi(argv[1]), res = 0;

	MSG_mailbox_set_async(hostN);

	xbt_mutex_acquire(m);
	atomics++;
	xbt_mutex_release(m);

	while(1)
	{
		msg_task_t task = NULL, new_task = NULL, new_task2 = NULL;
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if (strcmp(MSG_task_get_name(task), "End") == 0)
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		MSG_task_destroy(task);
		task = NULL;

		xbt_mutex_acquire(m);
		num_delt_exts++;
		num_event_exts++;
		xbt_mutex_release(m);

		if (depth != DEPTH - 1)
		{
			int next = (element + 1) % (WIDTH-1);
			if (next != 0)
			{
				new_task = MSG_task_create("0", 0, 0, NULL);
				sprintf(dest, "cp-%d-%d", depth, next);
				MSG_task_send(new_task, dest);

				new_task2 = MSG_task_create("End", 0, 0, NULL);
				sprintf(dest, "fin-0");
				MSG_task_send(new_task2, dest);
			}
			else
			{
				new_task = MSG_task_create("End", 0, 0, NULL);
				sprintf(dest, "fin-0");
				MSG_task_send(new_task, dest);
			}
		}
		else
		{
			new_task = MSG_task_create("End", 0, 0, NULL);
			sprintf(dest, "fin-0");
			MSG_task_send(new_task, dest);
		}
		xbt_mutex_acquire(m);
		num_delt_ints++;
		xbt_mutex_release(m);
	}
	return 0;
}

int generator(int argc, char *argv[])
{
	char* dest[30];
	m = xbt_mutex_init();

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			msg_task_t new_task = NULL;
			new_task = MSG_task_create("0", 0, 0, NULL);
			sprintf(dest, "cp-%d-%d", i, j);
			MSG_task_send(new_task, dest);

			if( i == DEPTH - 1 ) break;
		}
	}
	return 0;
}

int finisher(int argc, char *argv[])
{
	const char* hostN[30], dest[30];
	sprintf(hostN, "%s", argv[2]);
	long events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;
	int events_received = 0, res = 0;
	MSG_mailbox_set_async(hostN);

	while(1)
	{
		msg_task_t task = NULL, new_task = NULL;
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if (strcmp(MSG_task_get_name(task), "End") == 0)
		{
			MSG_task_destroy(task);
			task = NULL;
			events_received++;
			
			if (events_received == events)
			{
				for (int i = 0; i < DEPTH; i++){
					for (int j = 0; j < WIDTH-1; j++){
						new_task = MSG_task_create("End", 0, 0, NULL);
						sprintf(dest, "cp-%d-%d", i, j);
						MSG_task_send(new_task, dest);
						if(i == DEPTH - 1) break;
					}
				}
				break;
			}
		}
	}
	return 0;
}

