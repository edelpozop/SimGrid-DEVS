#include "./sim.h"

#define WIDTH 200
#define DEPTH 200

extern long int num_delt_ints;
extern long int num_delt_exts;
extern long int num_event_exts;
extern long int atomics;
extern sg_mutex_t m;

int atomic(int argc, char *argv[])
{
	char* hostN[30], dest[30];
	const char* sent_data = "0";
	sprintf(hostN, "%s", argv[2]);
	int depth = atoi(argv[0]), element = atoi(argv[1]), res = 0, recibidos = 0;

	sg_mailbox_t mailbox  = sg_mailbox_by_name(hostN);

	//sg_mutex_lock(m);
	atomics++;
	//sg_mutex_unlock(m);


	while(1)
	{
		char *received = (char*) sg_mailbox_get(mailbox);

		recibidos++;
		num_delt_exts++;
		num_event_exts++;

		//printf("%s %d\n", hostN, recibidos);

		//num_delt_ints++;
		//break;

		if (depth != DEPTH - 1)
		{
			int next = (element + 1) % (WIDTH-1);
			if (next != 0)
			{
				sprintf(dest, "cp-%d-%d", depth, next);
				sg_mailbox_t maildest = sg_mailbox_by_name(dest);
				sg_mailbox_put(maildest, sent_data, strlen(sent_data));
				//sg_mailbox_put_async(maildest, sent_data, strlen(sent_data));
				//sg_mailbox_put(maildest, NULL, NULL);
				num_delt_ints++;
				if(recibidos == element + 1) break;
			}
			else
			{
				num_delt_ints++;
				if(recibidos == element + 1) break;
			}
		}
		else
		{
			num_delt_ints++;
			if(recibidos == element + 1) break;
		}
	}
	return 0;
}

int generator(int argc, char *argv[])
{
	char* dest[30];

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			const char* sent_data = "0";
			sprintf(dest, "cp-%d-%d", i, j);
			sg_mailbox_t maildest = sg_mailbox_by_name(dest);
			sg_mailbox_put_async(maildest, sent_data, strlen(sent_data));
			if( i == DEPTH - 1 )break;
		}
	}
	return 0;
}

/*
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

*/
