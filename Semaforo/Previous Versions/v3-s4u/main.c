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

	simgrid_load_platform(file);
	simgrid_register_function("redLight", light);
	simgrid_register_function("greenLight", light);
	simgrid_register_function("yellowLight", light);

	sprintf(str, "redLight");
	argc = 2;
	char **argvc0 = xbt_new(char *, 3);
	argvc0[0] = bprintf("%s",str);
	argvc0[1] = bprintf("%g",simTime);
	argvc0[2] = NULL;

	sg_actor_create_(str, sg_host_by_name(str), light, argc, argvc0);

	sprintf(str, "greenLight");
	argc = 2;
	char **argvc1 = xbt_new(char *, 3);
	argvc1[0] = bprintf("%s",str);
	argvc1[1] = bprintf("%g",simTime);
	argvc1[2] = NULL;

	sg_actor_create_(str, sg_host_by_name(str), light, argc, argvc1);

	sprintf(str, "yellowLight");
	argc = 2;
	char **argvc2 = xbt_new(char *, 3);
	argvc2[0] = bprintf("%s",str);
	argvc2[1] = bprintf("%g",simTime);
	argvc2[2] = NULL;

	sg_actor_create_(str, sg_host_by_name(str), light, argc, argvc2);
	return;
}


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: %s platform_file simTime\n", argv[0]);
		exit(1);
	}

	simgrid_init(&argc, argv);

	double ts_start = get_time();
	test_all(argv[1], atof(argv[2]));
	simgrid_run();
	double ts_end = get_time();

	printf("Time - %g\n", ts_end - ts_start);
	else return 1;
}
