#include "./sim.h"

#define WIDTH 20
#define DEPTH 20

extern int num_delt_ints;
extern int num_delt_exts;
extern int num_event_exts;
extern int total_atomics;

xbt_mutex_t m;
//xbt_cond_t con;

int atomic(int argc, char *argv[])
{
	char* hostN[30], dest[30];
	sprintf(hostN, "%s", argv[2]);
	int depth = atoi(argv[0]), element = atoi(argv[1]), res = 0;

	MSG_mailbox_set_async(hostN);
	msg_host_t host = MSG_host_by_name(hostN);

	xbt_mutex_acquire(m);
	total_atomics++;
	xbt_mutex_release(m);

	while(1)
	{
		msg_task_t task = NULL, new_task = NULL;
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if (strcmp(MSG_task_get_name(task), "End") == 0)
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		xbt_mutex_acquire(m);
		num_delt_exts++;
		num_event_exts++;
		xbt_mutex_release(m);

		MSG_task_destroy(task);
		task = NULL;

		if (element != 0)
		{
			msg_task_t task2 = NULL;
			res = MSG_task_receive(&(task2), MSG_host_get_name(MSG_host_self()));
			if (res != MSG_OK) break;

			xbt_mutex_acquire(m);
			num_delt_exts++;
			num_event_exts++;
			xbt_mutex_release(m);

			MSG_task_destroy(task2);
			task2 = NULL;
		}
		if (depth != DEPTH - 1)
		{
			int next = (element + 1) % (WIDTH - 1);
			if (next != 0)
			{
				new_task = MSG_task_create("1", 0, 0, NULL);
				sprintf(dest, "cp-%d-%d", depth, next);
				MSG_task_send(new_task, dest);
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
		num_event_exts++;
		xbt_mutex_release(m);
	}
	return 0;
}

int generator(int argc, char *argv[])
{
	char* dest[30];

	m = xbt_mutex_init();
	//con = xbt_cond_init();

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH - 1; j++){
			msg_task_t new_task = NULL;

			if( i == DEPTH - 1 )
			{
				new_task = MSG_task_create("0", 0, 0, NULL);
				sprintf(dest, "cp-%d-0", i);
				MSG_task_send(new_task, dest);
				break;
			}
			else
			{
				new_task = MSG_task_create("0", 0, 0, NULL);
				sprintf(dest, "cp-%d-%d", i, j);
				MSG_task_send(new_task, dest);
			}
		}
	}
	return 0;
}

int finisher(int argc, char *argv[])
{
	const char* hostN[30], dest[30];
	sprintf(hostN, "%s", argv[2]);
	int total_messages = 0, res = 0;

	MSG_mailbox_set_async(hostN);
	msg_host_t host = MSG_host_by_name(hostN);

	while(1)
	{
		msg_task_t task = NULL, new_task = NULL;
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if (strcmp(MSG_task_get_name(task), "End") == 0)
		{
			MSG_task_destroy(task);
			task = NULL;
			total_messages++;
			if (total_messages == DEPTH)
			{
				for (int i = 0; i < DEPTH; i++){
					for (int j = 0; j < WIDTH - 1; j++){
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
	printf("%d\n", total_atomics);
	xbt_mutex_destroy(m);
	return 0;
}

