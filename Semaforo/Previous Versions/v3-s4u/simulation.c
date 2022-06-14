#include "./simulation.h"


int light(int argc, char * argv[]) {

  char mailbox[20], dest[20];
  int res = 0, init = 0;

  sprintf(mailbox, argv[0]);
  sg_mailbox_t mailbox  = sg_mailbox_by_name(hostN);

  while (1) {

    if(strcmp(argv[0],"redLight") && init == 0){
      init = 1;
      sprintf(dest, "greenLight");
      sg_mailbox_t maildest = sg_mailbox_by_name(dest);
      sg_mailbox_put_async(maildest, dest, strlen(dest));
    }
    if (simgrid_get_clock() > atof(argv[1]))break;
    char *received = (char*) sg_mailbox_get(mailbox);

    //printf("\033[0;31m");
    printf("Start %s - Time: %g\n", argv[0], simgrid_get_clock());

    if(strcmp(argv[0],"redLight") == 0)
    {
      sg_actor_sleep_for(DELTA_RED);
      sprintf(dest, "greenLight");
      sg_mailbox_t maildest = sg_mailbox_by_name(dest);
      sg_mailbox_put_async(maildest, dest, strlen(dest));
    }
    else if(strcmp(argv[0],"greenLight") == 0)
    {
      sg_actor_sleep_for(DELTA_GREEN);
      sprintf(dest, "yellowLight");
      sg_mailbox_t maildest = sg_mailbox_by_name(dest);
      sg_mailbox_put_async(maildest, dest, strlen(dest));
    }
    else{
      MSG_process_sleep(DELTA_YELLOW);
      sprintf(dest, "redLight");
      sg_mailbox_t maildest = sg_mailbox_by_name(dest);
      sg_mailbox_put_async(maildest, dest, strlen(dest));
    }
  }
  return 0;
}
