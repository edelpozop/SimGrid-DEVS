#include "./simulation.h"


int Light(int argc, char * argv[]) {

  msg_task_t task = NULL, new_task = NULL;
  char mailbox[20];
  int res = 0, init = 0;

  sprintf(mailbox, argv[0]);
  MSG_mailbox_set_async(mailbox);

  while (1) {

    if(strcmp(argv[0],"redLight") && init == 0){
      init = 1;
      char light[20];
      msg_task_t task = NULL;
      task = MSG_task_create("Green", 0, 0, NULL);

      sprintf(light, "greenLight");
      MSG_task_send(task, light);
    }

    if (MSG_get_clock() > atof(argv[1]))break;
    res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));

    if (res != MSG_OK) break;
    //printf("\033[0;31m");
    //printf("\n\nStart %s - Time: %g\n", argv[0], MSG_get_clock());

    if(strcmp(argv[0],"redLight") == 0)
    {
      MSG_process_sleep(DELTA_RED);
      MSG_task_destroy(task);
      task = NULL;

      new_task = MSG_task_create("Green", 0, 0, NULL);
      sprintf(mailbox, "greenLight");
      MSG_task_send(new_task, mailbox);
    }
    else if(strcmp(argv[0],"greenLight") == 0)
    {
      MSG_process_sleep(DELTA_GREEN);
      MSG_task_destroy(task);
      task = NULL;

      new_task = MSG_task_create("Yellow", 0, 0, NULL);
      sprintf(mailbox, "yellowLight");
      MSG_task_send(new_task, mailbox);
    }
    else{
      MSG_process_sleep(DELTA_YELLOW);
      MSG_task_destroy(task);
      task = NULL;

      new_task = MSG_task_create("Red", 0, 0, NULL);
      sprintf(mailbox, "redLight");
      MSG_task_send(new_task, mailbox);
    }
  }
  return 0;
}
