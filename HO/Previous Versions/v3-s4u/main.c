#include "./sim.h"
#include <sys/time.h>
#include <sys/resource.h>

#define WIDTH 200
#define DEPTH 200

long int num_delt_ints = 0;
long int num_delt_exts = 0;
long int num_event_exts = 0;
long int atomics = 0;

sg_mutex_t m;

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

	simgrid_load_platform(file);
	simgrid_register_function("atomic", atomic);
	simgrid_register_function("generator", generator);
	//MSG_function_register("finisher", finisher);

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			sprintf(str, "cp-%d-%d", i, j);
			argc = 3;
			char **argvc = xbt_new(char *, 4);
			argvc[0] = bprintf("%d", i);
			argvc[1] = bprintf("%d", j);
			argvc[2] = bprintf("%s", str);
			argvc[3] = NULL;
			sg_actor_create_(str, sg_host_by_name(str), atomic, argc, argvc);

			if(i == DEPTH - 1) break;
		}
	}

	sprintf(str, "gen-0");
	argc = 1;
	char **argvc1 = xbt_new(char *, 2);
	argvc1[0] = bprintf("%s", str);
	argvc1[1] = NULL;
	sg_actor_create_(str, sg_host_by_name(str), generator, argc, argvc1);

	return;

}

int main (int argc, char *argv[])
{
	struct rusage resources;

	double ts_start = get_time();
	simgrid_init(&argc, argv);
	//m = sg_mutex_init();
	//cond = xbt_cond_init();

	double ts_end_model_creation = get_time();
	creating_test(argv[1]);
	double ts_end_engine_setup = get_time();
	simgrid_run();
	double ts_end_simulation = get_time();

	double time_model = ts_end_model_creation - ts_start;
	double time_engine = ts_end_engine_setup - ts_end_model_creation;
	double time_sim = ts_end_simulation - ts_end_engine_setup;

	if (getrusage(RUSAGE_SELF, &resources)) perror ("rusage");

	long events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;
	//long events = ((WIDTH - 1) * (DEPTH - 1) + 1);
	printf("STATS\n");
	printf("Width: %d\nDepth: %d\n", WIDTH, DEPTH);
	printf("Num atomics: %d\n", atomics);
	printf("Num delta_int: %ld, [%ld]\n", num_delt_ints, events);
	printf("Num delta_ext: %ld, [%ld]\n", num_delt_exts, events);
	printf("Num event_ext: %ld, [%ld]\n", num_event_exts, events);
	printf("Model creation time (s):\t%.5f\n", time_model);
	printf("Engine setup time (s):\t\t%.5f\n", time_engine);
	printf("Simulation time (s):\t\t%.5f\n", time_sim);
	printf("TOTAL time (s):\t\t\t%.5f\n", time_model + time_engine + time_sim);
	printf("MEMORY (KiB):\t\t\t%ld\n", resources.ru_maxrss);

	exit(0);
}
