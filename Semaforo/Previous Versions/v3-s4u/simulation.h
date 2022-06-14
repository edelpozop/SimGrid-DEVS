#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <simgrid/actor.h>
#include <simgrid/engine.h>
#include <simgrid/host.h>
#include <simgrid/mailbox.h>
#include <xbt/log.h>
#include <xbt/sysdep.h>


#define DELTA_GREEN 60
#define DELTA_RED 57
#define DELTA_YELLOW 3


int light(int argc, char *argv[]);
void test_all(char *file, double simTime);
