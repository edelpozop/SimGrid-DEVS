#include "./simulation.h"

int init = 0;
xbt_mutex_t m ;
xbt_cond_t con;

int end = 0;
double time = 0.0;

int redLight(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[20];
	int res = 0;

	sprintf(mailbox, "redLight");
	MSG_mailbox_set_async(mailbox);

	while(1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if(res != MSG_OK) break;

		if(strcmp(MSG_task_get_name(task), "Finalize") == 0)
        {
        	MSG_task_destroy(task);
			task = NULL;
			break;
        }
		else if (strcmp(MSG_task_get_name(task), "Red") == 0)
        {
        	printf("Start Red - Time: %g\n", time);
        	MSG_process_sleep(DELTA_RED);

        	xbt_mutex_acquire(m);
			time += DELTA_RED;
			xbt_cond_signal(con);
			xbt_mutex_release(m);

        	MSG_task_destroy(task);
			task = NULL;
			new_task = MSG_task_create("Green", 0, 0, NULL);
			sprintf(mailbox, "greenLight");
			MSG_task_send(new_task, mailbox);
        }
	}

	endLight();

	return 0;
}

int greenLight(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[20];
	int res = 0;

	sprintf(mailbox, "greenLight");
	MSG_mailbox_set_async(mailbox);

	while(1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if(res != MSG_OK) break;

		if(strcmp(MSG_task_get_name(task), "Finalize") == 0)
        {
        	MSG_task_destroy(task);
			task = NULL;
			break;
        }
        else if (strcmp(MSG_task_get_name(task), "Green") == 0)
        {
        	printf("Start Green - Time: %g\n", time);
        	MSG_process_sleep(DELTA_GREEN);

        	xbt_mutex_acquire(m);
			time += DELTA_GREEN;
			xbt_cond_signal(con);
			xbt_mutex_release(m);

        	
        	MSG_task_destroy(task);
			task = NULL;
			new_task = MSG_task_create("Yellow", 0, 0, NULL);
			sprintf(mailbox, "yellowLight");
			MSG_task_send(new_task, mailbox);
        }
	}

	endLight();

	return 0;
}

int yellowLight(int argc, char *argv[])
{
	msg_task_t task = NULL, new_task = NULL;
	char mailbox[20];
	int res = 0;

	sprintf(mailbox, "yellowLight");
	MSG_mailbox_set_async(mailbox);

	while(1)
	{
		res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
		if(res != MSG_OK) break;

		if(strcmp(MSG_task_get_name(task), "Finalize") == 0)
        {
        	MSG_task_destroy(task);
			task = NULL;
			break;

        }
        else if (strcmp(MSG_task_get_name(task), "Yellow") == 0)
        {
        	printf("Start Yellow - Time: %g\n", time);
        	MSG_process_sleep(DELTA_YELLOW);

        	xbt_mutex_acquire(m);
			time += DELTA_YELLOW;
			xbt_cond_signal(con);
			xbt_mutex_release(m);
        	
        	MSG_task_destroy(task);
			task = NULL;
			new_task = MSG_task_create("Red", 0, 0, NULL);
			sprintf(mailbox, "redLight");
			MSG_task_send(new_task, mailbox);
        }
	}

	endLight();

	return 0;
}




int controller(int argc, char *argv[])
{
	msg_task_t task = NULL;
	char mailbox[64], light[30];
	int res = 0;
	
	sprintf(mailbox, "cont-0");
	MSG_mailbox_set_async(mailbox);

	init_m();

	while(1)
	{
		xbt_mutex_acquire(m);

		while (time <= 5000.0)
		{
			xbt_cond_wait(con, m);
		}
		
		xbt_mutex_release(m);

		task = MSG_task_create("Finalize", 0, 0, NULL);

		sprintf(light, "greenLight");
		MSG_task_send(task, light);
		
		task = MSG_task_create("Finalize", 0, 0, NULL);

		sprintf(light, "redLight");
		MSG_task_send(task, light);

		task = MSG_task_create("Finalize", 0, 0, NULL);

		sprintf(light, "yellowLight");
		MSG_task_send(task, light);

		break;
		
	}			
	return 0;
}


void init_m(void)
{

	if (init == 0) {
		init = 1;
		m = xbt_mutex_init();
		con = xbt_cond_init();

		char light[20];
		msg_task_t task = NULL;
		task = MSG_task_create("Green", 0, 0, NULL);

		sprintf(light, "greenLight");
		MSG_task_send(task, light);
	}

}


void endLight(void)
{
	xbt_mutex_acquire(m);
	end++;
	if (end == 3) printf("End time - %g\n", time);
	xbt_mutex_release(m);
}
