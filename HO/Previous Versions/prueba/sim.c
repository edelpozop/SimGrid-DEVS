#include "./sim.h"

#define WIDTH 200
#define DEPTH 200

extern long int num_delt_ints;
extern long int num_delt_exts;
extern long int num_event_exts;
extern long int atomics;
extern xbt_cond_t cond;
extern xbt_mutex_t m;

int atomic(int argc, char *argv[])
{
	long events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;
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

		num_delt_exts++;
		num_event_exts++;
		if (depth != DEPTH - 1)
		{
			int next = (element + 1) % (WIDTH-1);
			if (next != 0)
			{
				new_task = MSG_task_create("0", 0, 0, NULL);
				sprintf(dest, "cp-%d-%d", depth, next);
				MSG_task_send(new_task, dest);

				if(num_event_exts == events)
				{
					xbt_mutex_acquire(m);
					xbt_cond_signal(cond);
					xbt_mutex_release(m);
				}
			}
			else
			{
				if(num_event_exts == events)
				{
					xbt_mutex_acquire(m);
					xbt_cond_signal(cond);
					xbt_mutex_release(m);
				}
			}
		}
		else
		{
			if(num_event_exts == events)
			{
				xbt_mutex_acquire(m);
				xbt_cond_signal(cond);
				xbt_mutex_release(m);
			}
		}
		num_delt_ints++;
	}
	return 0;
}

int generator(int argc, char *argv[])
{
	char* dest[30];

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			msg_task_t new_task = NULL;
			new_task = MSG_task_create("0", 0, 0, NULL);
			sprintf(dest, "cp-%d-%d", i, j);
			MSG_task_send(new_task, dest);
			if( i == DEPTH - 1 )break;
		}
	}
	return 0;
}

int finisher(int argc, char *argv[])
{
	const char* dest[30];
	long events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;

	xbt_mutex_acquire(m);
	while(num_event_exts != events)
	{
		xbt_cond_wait(cond, m);
	}
	xbt_mutex_release(m);

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			msg_task_t new_task = NULL;
			new_task = MSG_task_create("End", 0, 0, NULL);
			sprintf(dest, "cp-%d-%d", i, j);
			MSG_task_send(new_task, dest);
			if( i == DEPTH - 1 )break;
		}
	}
	return 0;
}

