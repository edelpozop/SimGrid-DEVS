#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "simgrid/msg.h"
#include "xbt/synchro.h"

int atomic(int argc, char *argv[]);
int finisher(int argc, char *argv[]);
int generator(int argc, char *argv[]);
void creating_test(char *file);
