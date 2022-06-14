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


int redLight(int argc, char *argv[]);
int greenLight(int argc, char *argv[]);
int yellowLight(int argc, char *argv[]);
int toAutomatic(int argc, char *argv[]);
int toManual(int argc, char *argv[]);
int manual(int argc, char *argv[]);
int controller(int argc, char *argv[]);
void test_all(char *file);
