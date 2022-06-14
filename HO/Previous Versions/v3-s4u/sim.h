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

int atomic(int argc, char *argv[]);
int finisher(int argc, char *argv[]);
int generator(int argc, char *argv[]);
void creating_test(char *file);
