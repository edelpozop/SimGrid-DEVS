#include "./simulation_devs.h"
#include "./rand.h"



int init = 0;
xbt_mutex_t m;
xbt_cond_t con;
int end_total = 0;
int tot_toAutomatic = 0;
int tot_toManual = 0;

int manual(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[256];
	int res;

	sprintf(mailbox, "manual");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if ( strcmp(MSG_task_get_name(task), "Finalize") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		printf("Start manual - Time: %g\n", MSG_get_clock());
		MSG_process_sleep(DELTA_MANUAL);
		MSG_task_destroy(task);
		task = NULL;

		if ( messtoAutomatic() == 1 )
		{
			new_task = MSG_task_create("toAutomatic", 0, 0, NULL);
			sprintf(mailbox, "toAutomatic");
			MSG_task_send(new_task, mailbox);
		}
	}
	end();
	return 0;
}


int toAutomatic(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[256];
	int res;

	sprintf(mailbox, "toAutomatic");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if ( strcmp(MSG_task_get_name(task), "Finalize") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		printf("Start toAutomatic - Time: %g\n", MSG_get_clock());
		MSG_process_sleep(DELTA_TOAUTOMATIC);

		if ( strcmp(MSG_task_get_name(task), "toAutomatic") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			new_task = MSG_task_create("redLight", 0, 0, NULL);
			sprintf(mailbox, "redLight");
			MSG_task_send(new_task, mailbox);
		}
	}
	end();
	return 0;
}


int greenLight(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[256];
	int res;

	sprintf(mailbox, "greenLight");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if ( strcmp(MSG_task_get_name(task), "Finalize") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		printf("Start greenLight - Time: %g\n", MSG_get_clock());
		MSG_process_sleep(DELTA_GREENLIGHT);

		if ( strcmp(MSG_task_get_name(task), "greenLight") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;

			if ( messtoManual() == 1 )
			{
				new_task = MSG_task_create("toManual", 0, 0, NULL);
				sprintf(mailbox, "toManual");
				MSG_task_send(new_task, mailbox);
			}
			else if ( messtoManual() == 0 )
			{
				new_task = MSG_task_create("yellowLight", 0, 0, NULL);
				sprintf(mailbox, "yellowLight");
				MSG_task_send(new_task, mailbox);
			}
		}
	}
	end();
	return 0;
}


int yellowLight(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[256];
	int res;

	sprintf(mailbox, "yellowLight");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if ( strcmp(MSG_task_get_name(task), "Finalize") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		printf("Start yellowLight - Time: %g\n", MSG_get_clock());
		MSG_process_sleep(DELTA_YELLOWLIGHT);

		if ( strcmp(MSG_task_get_name(task), "yellowLight") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;

			if ( messtoManual() == 1 )
			{
				new_task = MSG_task_create("toManual", 0, 0, NULL);
				sprintf(mailbox, "toManual");
				MSG_task_send(new_task, mailbox);
			}
			else if ( messtoManual() == 0 )
			{
				new_task = MSG_task_create("redLight", 0, 0, NULL);
				sprintf(mailbox, "redLight");
				MSG_task_send(new_task, mailbox);
			}
		}
	}
	end();
	return 0;
}


int redLight(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[256];
	int res;

	sprintf(mailbox, "redLight");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if ( strcmp(MSG_task_get_name(task), "Finalize") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		printf("Start redLight - Time: %g\n", MSG_get_clock());
		MSG_process_sleep(DELTA_REDLIGHT);

		if ( strcmp(MSG_task_get_name(task), "redLight") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;

			if ( messtoManual() == 1 )
			{
				new_task = MSG_task_create("toManual", 0, 0, NULL);
				sprintf(mailbox, "toManual");
				MSG_task_send(new_task, mailbox);
			}
			else if ( messtoManual() == 0 )
			{
				new_task = MSG_task_create("greenLight", 0, 0, NULL);
				sprintf(mailbox, "greenLight");
				MSG_task_send(new_task, mailbox);
			}
		}
	}
	end();
	return 0;
}


int toManual(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[256];
	int res;

	sprintf(mailbox, "toManual");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if (res != MSG_OK) break;

		if ( strcmp(MSG_task_get_name(task), "Finalize") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			break;
		}

		printf("Start toManual - Time: %g\n", MSG_get_clock());
		MSG_process_sleep(DELTA_TOMANUAL);

		if ( strcmp(MSG_task_get_name(task), "toManual") == 0 )
		{
			MSG_task_destroy(task);
			task = NULL;
			new_task = MSG_task_create("manual", 0, 0, NULL);
			sprintf(mailbox, "manual");
			MSG_task_send(new_task, mailbox);
		}
	}
	end();
	return 0;
}


int controller (int argc, char *argv[])
{
	msg_task_t task = NULL;
	char mailbox[64], state[30];
	int res;

	sprintf(mailbox, "cont-0");
	MSG_mailbox_set_async(mailbox);

	init_m();

	while (1)
	{
		xbt_mutex_acquire(m);

		while (MSG_get_clock() <= 365 * 24 * 3600)
		{
			xbt_cond_wait(con, m);
		}

		xbt_mutex_release(m);

		task = MSG_task_create("Finalize", 0, 0, NULL);
		sprintf(state, "manual");
		MSG_task_send(task, state);

		task = MSG_task_create("Finalize", 0, 0, NULL);
		sprintf(state, "toAutomatic");
		MSG_task_send(task, state);

		task = MSG_task_create("Finalize", 0, 0, NULL);
		sprintf(state, "greenLight");
		MSG_task_send(task, state);

		task = MSG_task_create("Finalize", 0, 0, NULL);
		sprintf(state, "yellowLight");
		MSG_task_send(task, state);

		task = MSG_task_create("Finalize", 0, 0, NULL);
		sprintf(state, "redLight");
		MSG_task_send(task, state);

		task = MSG_task_create("Finalize", 0, 0, NULL);
		sprintf(state, "toManual");
		MSG_task_send(task, state);

		break;
	}
	return 0;
}

int init_m (void)
{
	if(init == 0)
	{
		init = 1;
		m = xbt_mutex_init();
		con = xbt_cond_init();

		char state[30];
		msg_task_t task = MSG_task_create("redLight", 0, 0, NULL);
		sprintf(state, "redLight");
		MSG_task_send(task, state);
	}
}

int end (void)
{
	xbt_mutex_acquire(m);
	end_total++;
	if (end_total == 6) printf("\n\nEND TIME - %g\n", MSG_get_clock());
	xbt_mutex_release(m);
}

int externalTransition (int argc, char *argv[])
{
	msg_task_t task = NULL;
	char mailbox[64], state[30];
	int res;

	sprintf(mailbox, "externalTransition");
	MSG_mailbox_set_async(mailbox);

	while (1)
	{
		int rand_time = rand() % 150 + 100;
		MSG_process_sleep(rand_time);

		
		xbt_mutex_acquire(m);
		tot_toAutomatic++;
		tot_toManual++;
		xbt_mutex_release(m);
	

		xbt_mutex_acquire(m);
		if (end_total == 6) 
		{
			xbt_mutex_release(m);
			break;
		}
		xbt_mutex_release(m);
	}
	return 0;
}

int messtoAutomatic()
{
	int local = 0;

	xbt_mutex_acquire(m);
	local = tot_toAutomatic;
	xbt_cond_signal(con);
	xbt_mutex_release(m);

	if ( local == 0 ) return 0;
	else
	{
		xbt_mutex_acquire(m);
		tot_toAutomatic--;
		xbt_mutex_release(m);
		return 1;
	}
}

int messtoManual()
{
	int local = 0;

	xbt_mutex_acquire(m);
	local = tot_toManual;
	xbt_cond_signal(con);
	xbt_mutex_release(m);

	if ( local == 0 ) return 0;
	else
	{
		xbt_mutex_acquire(m);
		tot_toManual--;
		xbt_mutex_release(m);
		return 1;
	}
}

