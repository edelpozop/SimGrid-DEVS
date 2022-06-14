#include "./sim.h"
#include <sys/time.h>
#include <sys/resource.h>

#define WIDTH 4
#define DEPTH 4

int num_delt_ints = 0;
int num_delt_exts = 0;
int num_event_exts = 0;
int atomics = 0;

void usage(char* name) {
  fprintf(stderr, "Usage: %s <width> <depth>\nwith w>=2, d>=2\n", name);
}

double get_time(void)
{
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return((double) tp.tv_sec + 0.000001 * (double) tp.tv_usec);
}


void creating_test(char *file)
{
	char str[50];
	int argc = 0;
	msg_process_t p;

	MSG_create_environment(file);
	MSG_function_register("atomic", atomic);
	MSG_function_register("coupled", coupled);
	MSG_function_register("generator", generator);
	MSG_function_register("finisher", finisher);

	for(int i = 0; i < DEPTH; i++){
		sprintf(str, "cp-%d-0", i);
		argc = 2;
		char **argvc1 = xbt_new(char *, 3);
		argvc1[0] = bprintf("%d", i);
		argvc1[1] = bprintf("%s", str);
		argvc1[2] = NULL;
		p = MSG_process_create_with_arguments(str, coupled, NULL, MSG_get_host_by_name(str), argc, argvc1);
	}

	for(int i = 0; i < DEPTH-1; i++){
		for(int j = 1; j < WIDTH; j++){
			sprintf(str, "cp-%d-%d", i, j);
			char **argvc2 = xbt_new(char *, 4);
			argc = 3;
			argvc2[0] = bprintf("%d", i);
			argvc2[1] = bprintf("%d", j);
			argvc2[2] = bprintf("%s", str);
			argvc2[3] = NULL;
			p = MSG_process_create_with_arguments(str, atomic, NULL, MSG_get_host_by_name(str), argc, argvc2);
		}
	}

	sprintf(str, "gen-0");
	argc = 1;
	char **argvc3 = xbt_new(char *, 4);
	argvc3[0] = bprintf("%s", str);
	argvc3[1] = NULL;
	p = MSG_process_create_with_arguments(str, generator, NULL, MSG_get_host_by_name(str), argc, argvc3);

	sprintf(str, "fin-0");
	argc = 1;
	char **argvc4 = xbt_new(char *, 2);
	argvc4[0] = bprintf("%s", str);
	argvc4[1] = NULL;
	p = MSG_process_create_with_arguments(str, finisher, NULL, MSG_get_host_by_name(str), argc, argvc4);

	return;

}

int main (int argc, char *argv[])
{
	msg_error_t res = MSG_OK;
	struct rusage resources;

	if (argc > 2)
	{
		printf("./%s platform_user.xml \n", argv[0]);
		exit(1);
	}

	double ts_start = get_time();
	MSG_init(&argc, argv);
	double ts_end_model_creation = get_time();
	creating_test(argv[1]);
	double ts_end_engine_setup = get_time();
	res = MSG_main();
	double ts_end_simulation = get_time();

	double time_model = ts_end_model_creation - ts_start;
	double time_engine = ts_end_engine_setup - ts_end_model_creation;
	double time_sim = ts_end_simulation - ts_end_engine_setup;

	if (getrusage(RUSAGE_SELF, &resources)) {
		perror ("rusage");
	}

	long events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;

	printf("STATS\n");
	printf("Width: %d\nDepth: %d\n", WIDTH, DEPTH);
	printf("Num atomics: %d\n", atomics);
	printf("Num delta_int: %d, [%d]\n", num_delt_ints, events);
	printf("Num delta_ext: %d, [%d]\n", num_delt_exts, events);
	printf("Num event_ext: %d, [%d]\n", num_event_exts, events);
	printf("Model creation time (s):\t%.5f\n", time_model);
	printf("Engine setup time (s):\t\t%.5f\n", time_engine);
	printf("Simulation time (s):\t\t%.5f\n", time_sim);
	printf("TOTAL time (s):\t\t\t%.5f\n", time_model + time_engine + time_sim);
	printf("MEMORY (KiB):\t\t\t%ld\n", resources.ru_maxrss);


	if ( res == MSG_OK) return 0;
	else return 1;
}
