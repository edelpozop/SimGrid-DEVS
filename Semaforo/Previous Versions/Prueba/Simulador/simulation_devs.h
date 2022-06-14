#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "simgrid/plugins/energy.h"
#include "simgrid/msg.h"
#include "xbt/synchro.h"

#define DELTA_MANUAL 15.0
#define DELTA_TOAUTOMATIC 0.0
#define DELTA_GREENLIGHT 57.0
#define DELTA_YELLOWLIGHT 3.0
#define DELTA_REDLIGHT 60.0
#define DELTA_TOMANUAL 0.0


int manual(int argc, char *argv[]);
int toAutomatic(int argc, char *argv[]);
int greenLight(int argc, char *argv[]);
int yellowLight(int argc, char *argv[]);
int redLight(int argc, char *argv[]);
int toManual(int argc, char *argv[]);
int controller(int argc, char *argv[]);
int externalTransition(int argc, char *argv[]);
