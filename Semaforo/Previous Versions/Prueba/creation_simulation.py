import json
import random


def main():

    with open('./Input/trafficlights.json') as f:
        data = json.load(f)

        sim = open("./Simulador/simulation_devs.c", "w+")

        sim.write('#include "./Simulador/simulation_devs.h"\n')
        sim.write('#include "./Simulador/rand.h"\n\n')

        sim.write('\n\nint init = 0;\n')
        sim.write('xbt_mutex_t m;\n')
        sim.write('xbt_cond_t con;\n')
        sim.write('int end = 0;\n')
        sim.write('double time = 0.0;\n\n')

        for j in data['atomicType']['TimeAdvance']['ta']:
            sim.write('int ' + str(j['state']) + '(int argc, char *argv[])\n{\n')
            sim.write('\tmsg_task_t task = NULL, new_task = NULL;\n')
            sim.write('\tchar mailbox[256];\n')
            sim.write('\tint res;\n\n')

            sim.write('\tsprintf(mailbox, "' + str(j['state']) + '");\n')
            sim.write('\tMSG_mailbox_set_async(mailbox);\n\n')
            sim.write('\twhile (1)\n\t{\n')
            sim.write('\t\tres = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));\n')
            sim.write('\t\tif (res != MSG_OK) break;\n\n')
            sim.write('\t\tif strcmp(MSG_task_get_name(task), "Finalize") == 0)\n\t\t{\n')
            sim.write('\t\t\tMSG_task_destroy(task);\n')
            sim.write('\t\t\ttask = NULL;\n')
            sim.write('\t\t\tbreak;\n\t\t}\n\n')

            sim.write('\t\tif strcmp(MSG_task_get_name(task), "' + str(j['state']) + '") == 0)\n\t\t{\n')
            sim.write('\t\t\tprintf("Start ' + str(j['state']) + ' - Time: %g\\n", time);\n')
            sim.write('\t\t\tMSG_process_sleep(DELTA_' + str(j['state']).upper() + ');\n\n')

            sim.write('\t\t\txbt_mutex_acquire(m);\n')
            sim.write('\t\t\ttime += DELTA_' + str(j['state']).upper() + ';\n')
            sim.write('\t\t\txbt_cond_signal(con);\n')
            sim.write('\t\t\txbt_mutex_release(m);\n\n')

            sim.write('\t\t\tMSG_task_destroy(task);\n')
            sim.write('\t\t\ttask = NULL;\n')

            for i in data['atomicType']['deltint']['InternalTransition']:
                if str(i['transition']['StartState']) == str(j['state']):
                    sim.write('\t\t\tnew_task = MSG_task_create("' + str(i['transition']['NextState']) + '", 0, 0, NULL);\n')
                    sim.write('\t\t\tsprintf(mailbox, "' + str(i['transition']['NextState']) + '");\n')
                    sim.write('\t\t\tMSG_task_send(new_task, mailbox);\n')
                    break

            sim.write('\t\t}\n\n')

            for i in data['atomicType']['deltext']['ExternalTransition']:
                if str(i['transition']['StartState']) == str(j['state']):
                    sim.write('\t\tif strcmp(MSG_task_get_name(task), "' + str(i['IncomingMessage']) + '") == 0)\n\t\t{\n')
                    sim.write('\t\t\tnew_task = MSG_task_create("' + str(i['transition']['NextState']) + '", 0, 0, NULL);\n')
                    sim.write('\t\t\tsprintf(mailbox, "' + str(i['transition']['NextState']) + '");\n')
                    sim.write('\t\t\tMSG_task_send(new_task, mailbox);\n\t\t}')
                    break


            sim.write('\n\t}\n\n')
            sim.write('\tend();\n')
            sim.write('\treturn 0;\n}\n\n')


        sim.write('int controller (int argc, char *argv[])\n{\n')
        sim.write('\tmsg_task_t task = NULL;\n')
        sim.write('\tchar mailbox[64], state[30];\n')
        sim.write('\tint res;\n\n')

        sim.write('\tsprintf(mailbox, "cont-0");\n')
        sim.write('\tMSG_mailbox_set_async(mailbox);\n\n')
        sim.write('\tinit_m();\n\n')

        sim.write('\twhile (1)\n\t{\n')
        sim.write('\t\txbt_mutex_acquire(m);\n\n')

        number = random.randint(3000, 10000)

        sim.write('\t\twhile (time <= ' + str(float(number)) + ')\n\t\t}\n')
        sim.write('\t\t\txbt_cond_wait(con, m);\n\t\t}\n\n')
        sim.write('\t\txbt_mutex_release(m);\n\n')

        for j in data['atomicType']['TimeAdvance']['ta']:
            sim.write('\t\ttask = MSG_task_create("Finalize", 0, 0, NULL);\n')
            sim.write('\t\tsprintf(state, "' + str(j['state']) + '");\n')
            sim.write('\t\tMSG_task_send(task, state);\n\n')

        sim.write('\t\tbreak;\n\t}\n\treturn 0;\n}\n\n')



        sim.write('int init_m (void)\n{\n')
        sim.write('\tif(init == 0)\n\t{\n')
        sim.write('\t\tinit = 1;\n')
        sim.write('\t\tm = xbt_mutex_init();\n')
        sim.write('\t\tcon = xbt_cond_init();\n\n')
        sim.write('\t\tchar state[30];\n')

        for i in data['atomicType']['deltint']['InternalTransition']:
            if i['-intTransitionID'] == '1':
                sim.write('\t\ttask = msg_task_create("' + str(i['transition']['StartState']) + '", 0, 0, NULL);\n')
                sim.write('\t\tsprintf(state, "' + str(i['transition']['StartState']) + '");\n')
                sim.write('\t\tMSG_task_send(task, state);\n\t}\n}\n\n')
                break

        sim.write('int end (void)\n{\n')

        sim.write('\txbt_mutex_acquire(m);\n')
        sim.write('\tend++\n')
        sim.write('\tif (end == ' + str(len(data['atomicType']['TimeAdvance']['ta'])) + ') printf("\\n\\nEND TIME - %g\\n", time);\n')
        sim.write('\txbt_mutex_release(m);\n}\n\n')


        sim.write('int externalTransition (int argc, char *argv[])\n{\n')
        sim.write('\tmsg_task_t task = NULL;\n')
        sim.write('\tchar mailbox[64], state[30];\n')
        sim.write('\tint res;\n\n')

        sim.write('\tsprintf(mailbox, "externalTransition");\n')
        sim.write('\tMSG_mailbox_set_async(mailbox);\n\n')

        number = random.randint(20, 100)
        destinies = len(data['atomicType']['deltext']['ExternalTransition'])
        sim.write('\tfor (int i = 0; i < ' + str(number) + '; i++){\n')
        sim.write('\t\tint sent = 0, choice = rand() % ' + str(destinies) + ';\n\n')
        index = 0

        for i in data['atomicType']['deltext']['ExternalTransition']:
            sim.write('\t\tif ( choice == ' + str(index) + ' && sent == 0 )\n\t\t{\n')
            sim.write('\t\t\tsent = 1;\n')
            sim.write('\t\t\ttask = MSG_task_create("' + str(i['IncomingMessage']) + '", 0, 0, NULL);\n')
            sim.write('\t\t\tsprintf(state, "' + str(i['transition']['StartState']) + '");\n')
            sim.write('\t\t\tMSG_task_send(task, state);\n\n\t\t}\n\n')
            index = index + 1

        sim.write('\t}\n\treturn 0;\n}\n\n')

        sim.close()

if __name__ == "__main__":
    main()