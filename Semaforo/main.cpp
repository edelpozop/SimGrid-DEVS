// Versión 4 - S4U - 3.31 SimGrid

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "simgrid/s4u.hpp"
#include <cstdlib>
#include <iostream>

namespace s4g = simgrid::s4u;

int init = 0;

#define DELTA_GREEN 60
#define DELTA_RED 57
#define DELTA_YELLOW 3

double simTime = 0.0;

double get_time(void)
{
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return((double) tp.tv_sec + 0.000001 * (double) tp.tv_usec);
}

static void finalizer(){
	s4g::this_actor::sleep_for(simTime);
	s4g::Actor::kill_all();
	s4g::this_actor::exit();
}


static void light(const std::string& mailbox_name){
  auto* msgdest = new std::string ("a");
	s4g::Mailbox* mbox = s4g::Mailbox::by_name(mailbox_name);

	while(1)
	{
		if(mailbox_name.compare("redLight") == 0 && init == 0){
      init = 1;
			auto* msg = new std::string ("greenLight");
      s4g::Mailbox* out	= s4g::Mailbox::by_name("greenLight");
      out->put(msg, msg->size());
    }

    try{
			auto* msgreceived = mbox->get<std::string>();
    }catch (const simgrid::Exception& e) {
			printf("ERROR:: %s\n",e);
			break;
    }
    //if (s4g::Engine::get_clock() >= simTime) break;

		if(mailbox_name.compare("redLight") == 0)
    {
      s4g::this_actor::sleep_for(DELTA_RED);
      s4g::Mailbox* mailout	= s4g::Mailbox::by_name("greenLight");
  		mailout->put(msgdest, msgdest->size());
    }
    else if(mailbox_name.compare("greenLight") == 0)
    {
      s4g::this_actor::sleep_for(DELTA_GREEN);
      s4g::Mailbox* mailout	= s4g::Mailbox::by_name("yellowLight");
  		mailout->put(msgdest, msgdest->size());
    }
    else{
      s4g::this_actor::sleep_for(DELTA_YELLOW);
      s4g::Mailbox* mailout	= s4g::Mailbox::by_name("redLight");
  		mailout->put(msgdest, msgdest->size());
    }
	}
}

int main(int argc, char *argv[]){
	if (argc < 3){
		printf("Usage: %s platform_file simTime\n", argv[0]);
		exit(1);
	}
	simTime = double(atof(argv[2]));
	s4g::Engine e(&argc, argv);

	double ts_start = get_time();
	e.load_platform(argv[1]);
  s4g::Actor::create("redLight", e.host_by_name("redLight"), &light, "redLight");
  s4g::Actor::create("greenLight", e.host_by_name("greenLight"), &light, "greenLight");
  s4g::Actor::create("yellowLight", e.host_by_name("yellowLight"), &light, "yellowLight");
	s4g::Actor::create("finalizer", e.host_by_name("finalizer"), &finalizer);
	e.run();
	double ts_end = get_time();
	printf("%g\t%g\n", s4g::Engine::get_clock(), (ts_end - ts_start)*1000);

	return 0;
}
