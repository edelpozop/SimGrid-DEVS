#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "simgrid/plugins/energy.h"
#include "simgrid/msg.h"
#include "xbt/synchro.h"


#define DELTA_GREEN 60
#define DELTA_RED 47
#define DELTA_YELLOW 5


struct intTransition
{
	char id_task[40];
};


int redLight(int argc, char *argv[]);
int greenLight(int argc, char *argv[]);
int yellowLight(int argc, char *argv[]);
int controller(int argc, char *argv[]);
void test_all(char *file);
