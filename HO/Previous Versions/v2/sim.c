#include "./sim.h"

#define WIDTH 4
#define DEPTH 4

extern int num_delt_ints;
extern int num_delt_exts;
extern int num_event_exts;
extern int atomics;
xbt_mutex_t m;

int coupled(int argc, char *argv[])
{
	char* hostN[30], dest[30];
	sprintf(hostN, "%s", argv[1]);
	int depthC = atoi(argv[0]), res = 0;

	MSG_mailbox_set_async(hostN);
	msg_host_t host = MSG_host_by_name(hostN);

	while(1)
	{
		msg_task_t task = NULL, new_task = NULL;
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));

		if (strcmp(MSG_task_get_name(task), "End") == 0)
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		MSG_task_destroy(task);
		task = NULL;

		if( depthC == DEPTH - 1 )
		{
			xbt_mutex_acquire(m);
			atomics++;
			num_delt_exts++;
			num_event_exts++;
			num_delt_ints++;
			xbt_mutex_release(m);

			//printf("A host %s termina\n", hostN);
			new_task = MSG_task_create("End", 0, 0, NULL);
			sprintf(dest, "fin-0");
			MSG_task_send(new_task, dest);
			break;
		}

		for (int j = 1; j < WIDTH; j++){
			new_task = MSG_task_create("0", 0, 0, NULL);
			sprintf(dest, "cp-%d-%d", depthC, j);
			//printf("B host %s envia a %s\n", hostN, dest);
			MSG_task_send(new_task, dest);

			xbt_mutex_acquire(m);
			num_delt_ints++;
			xbt_mutex_release(m);
		}

		new_task = MSG_task_create("0", 0, 0, NULL);
		sprintf(dest, "cp-%d-0", depthC + 1);
		MSG_task_send(new_task, dest);
/*
		xbt_mutex_acquire(m);
		num_delt_ints++;
		xbt_mutex_release(m);
*/
		break;
	}
}



int atomic(int argc, char *argv[])
{
	char* hostN[30], dest[30];
	sprintf(hostN, "%s", argv[2]);
	int depth = atoi(argv[0]), element = atoi(argv[1]), res = 0;

	MSG_mailbox_set_async(hostN);
	msg_host_t host = MSG_host_by_name(hostN);

	xbt_mutex_acquire(m);
	atomics++;
	xbt_mutex_release(m);

	while(1)
	{
		msg_task_t task = NULL, new_task = NULL;
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));

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

		if (element != 1)
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
		
		int next = (element + 1) % (WIDTH);
		if (next != 0)
		{
			new_task = MSG_task_create("1", 0, 0, NULL);
			sprintf(dest, "cp-%d-%d", depth, next);
			MSG_task_send(new_task, dest);
			//printf("C host %s envia a %s\n", hostN, dest);
			xbt_mutex_acquire(m);
			num_delt_ints++;
			xbt_mutex_release(m);
		}
		else
		{
			new_task = MSG_task_create("End", 0, 0, NULL);
			sprintf(dest, "fin-0");
			MSG_task_send(new_task, dest);
		}
	}
	return 0;
}

int generator(int argc, char *argv[])
{
	char* dest[30];
	msg_task_t new_task = NULL;
	m = xbt_mutex_init();

	new_task = MSG_task_create("0", 0, 0, NULL);
	sprintf(dest, "cp-0-0");
	MSG_task_send(new_task, dest);

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
				for (int i = 0; i < DEPTH - 1; i++){
					for (int j = 0; j < WIDTH; j++){
						new_task = MSG_task_create("End", 0, 0, NULL);
						sprintf(dest, "cp-%d-%d", i, j);
						MSG_task_send(new_task, dest);
					}
				}
				break;
			}
		}
	}
	xbt_mutex_destroy(m);
	return 0;
}

