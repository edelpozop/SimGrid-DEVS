// Version 4 - S4U - SimGrid 3.31

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "simgrid/s4u.hpp"
#include <cstdlib>
#include <iostream>

#define WIDTH 200
#define DEPTH 200

namespace s4g = simgrid::s4u;

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

static void atomic (int idDepth, int idWidth)
{
	auto* msgdest = new std::string ("a");
	int depth = idDepth, element = idWidth, res = 0, recibidos = 0;

	auto* mailb = new std::string ("cp-"+std::to_string(depth)+"-"+std::to_string(element));
	s4g::Mailbox* mbox = s4g::Mailbox::by_name(mailb->c_str());

	atomics++;

	while(1)
	{
		auto* msgreceived = mbox->get<std::string>();

		recibidos++;
		num_delt_exts++;
		num_event_exts++;

		if (depth != DEPTH - 1)
		{
			int next = (element + 1) % (WIDTH - 1);
			if (next != 0)
			{
				auto* dest = new std::string ("cp-"+std::to_string(depth)+"-"+std::to_string(next));
				//sprintf(dest, "cp-%d-%d", depth, next);
				s4g::Mailbox* mailout	= s4g::Mailbox::by_name(dest->c_str());

  			mailout->put(msgdest, msgdest->size());
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
}

static void generator (const std::string& mailbox_name)
{
	auto* msgdest = new std::string ("a");

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			auto* dest = new std::string ("cp-" + std::to_string(i) + "-" + std::to_string(j));
			//sprintf(destination, "cp-%d-%d", i, j);
			s4g::Mailbox* out	= s4g::Mailbox::by_name(dest->c_str());
      out->put(msgdest, msgdest->size());

			if( i == DEPTH - 1 )break;
		}
	}
}


int main (int argc, char *argv[])
{
	struct rusage resources;
	char str[50];
	double ts_start = get_time();
	s4g::Engine e(&argc, argv);

	double ts_end_model_creation = get_time();
	e.load_platform(argv[1]);

	for(int i = 0; i < DEPTH; i++){
		for(int j = 0; j < WIDTH-1; j++){
			sprintf(str, "cp-%d-%d", i, j);
			//printf("host: %s\n", str);
			s4g::Actor::create(str, e.host_by_name(str), &atomic, i, j);
			if(i == DEPTH - 1) break;
		}
	}

  s4g::Actor::create("gen-0", e.host_by_name("gen-0"), &generator, "gen-0");

	double ts_end_engine_setup = get_time();
	e.run();
	double ts_end_simulation = get_time();

	double time_model = ts_end_model_creation - ts_start;
	double time_engine = ts_end_engine_setup - ts_end_model_creation;
	double time_sim = ts_end_simulation - ts_end_engine_setup;

	if (getrusage(RUSAGE_SELF, &resources)) perror ("rusage");

	long events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;
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
