#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "simgrid/plugins/energy.h"
#include "simgrid/msg.h"
#include "xbt/synchro.h"


#define DELTA_GREEN 60
#define DELTA_RED 57
#define DELTA_YELLOW 3


int Light(int argc, char *argv[]);
void test_all(char *file, double simTime);
