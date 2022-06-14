#include "./simulation.h"
#include <sys/time.h>

double get_time(void)
{
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return((double) tp.tv_sec + 0.000001 * (double) tp.tv_usec);
}

void test_all(char *file, double simTime)
{
	int argc;
	char str[50];
	msg_process_t p;

	MSG_create_environment(file);
	MSG_function_register("redLight", Light);
	MSG_function_register("greenLight", Light);
	MSG_function_register("yellowLight", Light);

	sprintf(str, "redLight");

	argc = 2;
	char **argvc0 = xbt_new(char *, 3);
	argvc0[0] = bprintf("%s",str);
	argvc0[1] = bprintf("%g",simTime);
	argvc0[2] = NULL;

	p = MSG_process_create_with_arguments(str, Light, NULL, MSG_get_host_by_name(str), argc, argvc0);
	if(p == NULL)
	{
		printf("Error en ......... %s\n", str);
		exit(0);
	}

	sprintf(str, "greenLight");
	argc = 2;
	char **argvc1 = xbt_new(char *, 3);
	argvc1[0] = bprintf("%s",str);
	argvc1[1] = bprintf("%g",simTime);
	argvc1[2] = NULL;

	p = MSG_process_create_with_arguments(str, Light, NULL, MSG_get_host_by_name(str), argc, argvc1);
	if(p == NULL)
	{
		printf("Error en ......... %s\n", str);
		exit(0);
	}


	sprintf(str, "yellowLight");
	argc = 2;
	char **argvc2 = xbt_new(char *, 3);
	argvc2[0] = bprintf("%s",str);
	argvc2[1] = bprintf("%g",simTime);
	argvc2[2] = NULL;

	p = MSG_process_create_with_arguments(str, Light, NULL, MSG_get_host_by_name(str), argc, argvc2);
	if(p == NULL)
	{
		printf("Error en ......... %s\n", str);
		exit(0);
	}

	return;
}


int main(int argc, char *argv[])
{
	msg_error_t res = MSG_OK;

	if (argc < 3)
	{
		printf("Usage: %s platform_file simTime", argv[0]);
		printf("\n");
		exit(1);
	}

	MSG_init(&argc, argv);

	double ts_start = get_time();
	test_all(argv[1], atof(argv[2]));
	res = MSG_main();
	double ts_end = get_time();

	printf("Time - %g\n", ts_end - ts_start);
	if (res == MSG_OK) return 0;
	else return 1;
}
