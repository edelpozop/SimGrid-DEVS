import csv, sys, os, datetime
import xml.etree.ElementTree as xml
import xml.dom.minidom as xmldom

def main():
    if len(sys.argv) != 3:
        print("python3 " + str(sys.argv[0]) + " <width> <depth>") 
        exit(0)

    width = int(sys.argv[1])
    depth = int(sys.argv[2])

    platform = xml.Element("platform")
    platform.set('version', '4.1')

    AS = xml.SubElement(platform, 'AS')
    AS.set('id', 'AS0')
    AS.set('routing', 'Full')

    for i in range(depth):
        if i == depth - 1:
            rad = "0-1"
        else:
            rad = "0-" + str(width - 1)

        ida = "Coupled-" + str(i)
        prefix = "cp-" + str(i) + "-"

        coupled = xml.SubElement(AS, 'cluster')
        coupled.set('id', ida)
        coupled.set('prefix', prefix)
        coupled.set('suffix', "")
        coupled.set('radical', rad)
        coupled.set('speed', '1e9f')
        coupled.set('bw', '1Gbps')
        coupled.set('lat', '0')
        coupled.set('router_id','RCoupled' + str(i))

    gen = xml.SubElement(AS, 'cluster')
    gen.set('id', 'Generator0')
    gen.set('prefix', 'gen-')
    gen.set('suffix', '')
    gen.set('radical', '0-1')
    gen.set('speed', '1e9f')
    gen.set('bw', '1Gbps')
    gen.set('lat', '0')
    gen.set('router_id','RGenerator0')

    fin = xml.SubElement(AS, 'cluster')
    fin.set('id', 'Finisher0')
    fin.set('prefix', 'fin-')
    fin.set('suffix', '')
    fin.set('radical', '0-1')
    fin.set('speed', '1e9f')
    fin.set('bw', '1Gbps')
    fin.set('lat', '0')
    fin.set('router_id','RFinisher0')

    for i in range(depth):
        link = xml.SubElement(AS, 'link')
        idl = "linkGen0Coupled" + str(i)
        link.set('id', idl)
        link.set('latency', '0')
        link.set('bandwidth', '1Gbps')

        link = xml.SubElement(AS, 'link')
        idl = "linkCoupled" + str(i) + "Fin0"
        link.set('id', idl)
        link.set('latency', '0')
        link.set('bandwidth', '1Gbps')

    for i in range(depth):
        ASroute = xml.SubElement(AS, 'ASroute')
        ASroute.set('src', 'Generator0')
        ASroute.set('dst', "Coupled-" + str(i))
        ASroute.set('gw_src', 'RGenerator0')
        ASroute.set('gw_dst', "RCoupled" + str(i))

        link_ctn = xml.SubElement(ASroute, 'link_ctn')
        link_ctn.set('id', "linkGen0Coupled" + str(i))

        ASroute = xml.SubElement(AS, 'ASroute')
        ASroute.set('src', "Coupled-" + str(i))
        ASroute.set('dst', 'Finisher0')
        ASroute.set('gw_src', "RCoupled" + str(i))
        ASroute.set('gw_dst', 'RFinisher0')

        link_ctn = xml.SubElement(ASroute, 'link_ctn')
        link_ctn.set('id', "linkCoupled" + str(i) + "Fin0")

    tree = xml.ElementTree(platform)

    with open("platform.xml", "wb") as fh:
        fh.write("<?xml version='1.0'?><!DOCTYPE platform SYSTEM 'http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd'>".encode('utf-8'))

    with open("platform.xml", "ab") as fh:
        tree.write(fh, encoding='utf-8')

    dom = xmldom.parse("platform.xml")
    string = dom.toprettyxml()

    f = open("platform.xml", "w")
    string = string.replace('<?xml version="1.0" ?>','<?xml version="1.0"?>')
    f.write(string)
    f.close()

    with open("main.c", "w+") as main:
        main.write('#include "./sim.h"\n')
        main.write('#include <sys/time.h>\n')
        main.write('#include <sys/resource.h>\n\n')

        main.write('#define WIDTH ' + str(width) + '\n')
        main.write('#define DEPTH ' + str(depth) + '\n\n')


        main.write('long int num_delt_ints = 0;\n')
        main.write('long int num_delt_exts = 0;\n')
        main.write('long int num_event_exts = 0;\n')
        main.write('long int atomics = 0;\n\n')
        main.write('xbt_mutex_t m;\n')
        main.write('xbt_cond_t cond;\n\n')

        main.write('double get_time(void)\n{\n')
        main.write('\tstruct timeval tp;\n')
        main.write('\tstruct timezone tzp;\n')
        main.write('\tgettimeofday(&tp, &tzp);\n')
        main.write('\treturn((double) tp.tv_sec + 0.000001 * (double) tp.tv_usec);\n}\n\n')


        main.write('\nvoid creating_test(char *file)\n{\n')
        main.write("\tchar str[50];\n")
        main.write('\tint argc = 0;\n')
        main.write("\tmsg_process_t p;\n\n")
        main.write("\tMSG_create_environment(file);\n")

        main.write('\tMSG_function_register("atomic", atomic);\n')
        main.write('\tMSG_function_register("generator", generator);\n')  
        main.write('\tMSG_function_register("finisher", finisher);\n\n') 

        main.write('\tfor(int i = 0; i < DEPTH; i++){\n')
        main.write('\t\tfor(int j = 0; j < WIDTH-1; j++){\n')
        main.write('\t\t\tsprintf(str, "cp-%d-%d", i, j);\n')
        main.write('\t\t\targc = 3;\n')
        main.write('\t\t\tchar **argvc = xbt_new(char *, 4);\n')
        main.write('\t\t\targvc[0] = bprintf("%d", i);\n')
        main.write('\t\t\targvc[1] = bprintf("%d", j);\n')
        main.write('\t\t\targvc[2] = bprintf("%s", str);\n')
        main.write('\t\t\targvc[3] = NULL;\n')
        main.write('\t\t\tp = MSG_process_create_with_arguments(str, atomic, NULL, MSG_get_host_by_name(str), argc, argvc);\n\n')
        main.write('\t\t\tif(p == NULL)\n\t\t\t{\n')
        main.write('\t\t\t\tprintf("Error en %s\\n", str);\n')
        main.write('\t\t\t\texit(0);\n\t\t\t}\n')
        main.write('\t\t\tif(i == DEPTH - 1) break;\n')
        main.write('\t\t}\n\t}\n\n')

        main.write('\tsprintf(str, "gen-0");\n')
        main.write('\targc = 1;\n')
        main.write('\tchar **argvc1 = xbt_new(char *, 2);\n')
        main.write('\targvc1[0] = bprintf("%s", str);\n')
        main.write('\targvc1[1] = NULL;\n')
        main.write('\tp = MSG_process_create_with_arguments(str, generator, NULL, MSG_get_host_by_name(str), argc, argvc1);\n\n')
        main.write('\tif(p == NULL)\n\t{\n')
        main.write('\t\tprintf("Error en %s\\n", str);\n')
        main.write('\t\texit(0);\n\t}\n\n')

        main.write('\tsprintf(str, "fin-0");\n')
        main.write('\targc = 1;\n')
        main.write('\tchar **argvc2 = xbt_new(char *, 2);\n')
        main.write('\targvc2[0] = bprintf("%s", str);\n')
        main.write('\targvc2[1] = NULL;\n')
        main.write('\tp = MSG_process_create_with_arguments(str, finisher, NULL, MSG_get_host_by_name(str), argc, argvc2);\n\n')
        main.write('\tif( p == NULL )\n\t{\n')
        main.write('\t\tprintf("Error en %s\\n", str);\n')
        main.write('\t\texit(0);\n\t}\n')

        main.write('\treturn;\n\n}\n\n')

        main.write('int main (int argc, char *argv[])\n{\n')
        main.write('\tmsg_error_t res = MSG_OK;\n')
        main.write('\tstruct rusage resources;\n\n')
        main.write('\tif (argc > 3)\n\t{\n')
        main.write('\t\tprintf("./%s platform_user.xml \\n", argv[0]);\n')
        main.write('\t\texit(1);\n\t}\n\n')
        main.write('\tdouble ts_start = get_time();\n')
        main.write('\tMSG_init(&argc, argv);\n')
        main.write('\tm = xbt_mutex_init();\n')
        main.write('\tcond = xbt_cond_init();\n\n')
        main.write('\tdouble ts_end_model_creation = get_time();\n')
        main.write('\tcreating_test(argv[1]);\n')
        main.write('\tdouble ts_end_engine_setup = get_time();\n')
        main.write('\tres = MSG_main();\n')
        main.write('\tdouble ts_end_simulation = get_time();\n\n')

        main.write('\tdouble time_model = ts_end_model_creation - ts_start;\n')
        main.write('\tdouble time_engine = ts_end_engine_setup - ts_end_model_creation;\n')
        main.write('\tdouble time_sim = ts_end_simulation - ts_end_engine_setup;\n\n')

        main.write('\tif (getrusage(RUSAGE_SELF, &resources))\n\t{\n')
        main.write('\t\tperror ("rusage");\n\t}\n\n')

        main.write('\tlong events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;\n\n')
        main.write('\tprintf("STATS\\n");\n')
        main.write('\tprintf("Width: %d\\nDepth: %d\\n", WIDTH, DEPTH);\n')
        main.write('\tprintf("Num atomics: %d\\n", atomics);\n')
        main.write('\tprintf("Num delta_int: %ld, [%ld]\\n", num_delt_ints, events);\n')
        main.write('\tprintf("Num delta_ext: %ld, [%ld]\\n", num_delt_exts, events);\n')
        main.write('\tprintf("Num event_ext: %ld, [%ld]\\n", num_event_exts, events);\n')
        main.write('\tprintf("Model creation time (s):\\t%.5f\\n", time_model);\n')
        main.write('\tprintf("Engine setup time (s):\\t\\t%.5f\\n", time_engine);\n')
        main.write('\tprintf("Simulation time (s):\\t\\t%.5f\\n", time_sim);\n')
        main.write('\tprintf("TOTAL time (s):\\t\\t\\t%.5f\\n", time_model + time_engine + time_sim);\n')
        main.write('\tprintf("MEMORY (KiB):\\t\\t\\t%ld\\n", resources.ru_maxrss);\n')
        main.write('\tif ( res == MSG_OK) return 0;\n')
        main.write('\telse return 1;\n}\n')

        

    with open("sim.c", "w+") as sim:
        sim.write('#include "./sim.h"\n\n')

        sim.write('#define WIDTH ' + str(width) + '\n')
        sim.write('#define DEPTH ' + str(depth) + '\n\n')

        sim.write('extern long int num_delt_ints;\n')
        sim.write('extern long int num_delt_exts;\n')
        sim.write('extern long int num_event_exts;\n')
        sim.write('extern long int atomics;\n')
        sim.write('extern xbt_cond_t cond;\n')
        sim.write('extern xbt_mutex_t m;\n\n')


        sim.write('int atomic(int argc, char *argv[])\n{\n')
        sim.write('\tlong events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;\n')
        sim.write('\tchar* hostN[30], dest[30];\n')
        sim.write('\tsprintf(hostN, "%s", argv[2]);\n')
        sim.write('\tint depth = atoi(argv[0]), element = atoi(argv[1]), res = 0;\n\n')
        sim.write('\tMSG_mailbox_set_async(hostN);\n\n')
        sim.write('\txbt_mutex_acquire(m);\n')
        sim.write('\tatomics++;\n')
        sim.write('\txbt_mutex_release(m);\n\n')
        sim.write('\twhile(1)\n\t{\n')
        sim.write('\t\tmsg_task_t task = NULL, new_task = NULL, new_task2 = NULL;\n')
        sim.write('\t\tres = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));\n')
        sim.write('\t\tif (res != MSG_OK) break;\n\n')
        sim.write('\t\tif (strcmp(MSG_task_get_name(task), "End") == 0)\n\t\t{\n')
        sim.write('\t\t\tMSG_task_destroy(task);\n')
        sim.write('\t\t\ttask = NULL;\n')
        sim.write('\t\t\tbreak;\n\t\t}\n\n')

        sim.write('\t\tMSG_task_destroy(task);\n')
        sim.write('\t\ttask = NULL;\n\n')

        #sim.write('\t\txbt_mutex_acquire(m);\n')
        sim.write('\t\tnum_delt_exts++;\n')
        sim.write('\t\tnum_event_exts++;\n')
        #sim.write('\t\txbt_mutex_release(m);\n\n')
        
        sim.write('\t\tif (depth != DEPTH - 1)\n\t\t{\n')
        sim.write('\t\t\tint next = (element + 1) % (WIDTH-1);\n')
        sim.write('\t\t\tif (next != 0)\n\t\t\t{\n')
        sim.write('\t\t\t\tnew_task = MSG_task_create("0", 0, 0, NULL);\n')
        sim.write('\t\t\t\tsprintf(dest, "cp-%d-%d", depth, next);\n')
        sim.write('\t\t\t\tMSG_task_send(new_task, dest);\n\n')
        sim.write('\t\t\t\tif(num_event_exts == events)\n\t\t\t\t{\n')
        sim.write('\t\t\t\t\txbt_mutex_acquire(m);\n')
        sim.write('\t\t\t\t\txbt_cond_signal(cond);\n')
        sim.write('\t\t\t\t\txbt_mutex_release(m);\n')
        sim.write('\t\t\t\t}\n') 
        sim.write('\t\t\t}\n')
        #sim.write('\t\t\t\tnew_task2 = MSG_task_create("End", 0, 0, NULL);\n')
        #sim.write('\t\t\t\tsprintf(dest, "fin-0");\n')
        #sim.write('\t\t\t\tMSG_task_send(new_task2, dest);\n\t\t\t}\n')
        sim.write('\t\t\telse\n\t\t\t{\n')
        sim.write('\t\t\t\tif(num_event_exts == events)\n\t\t\t\t{\n')
        sim.write('\t\t\t\t\txbt_mutex_acquire(m);\n')
        sim.write('\t\t\t\t\txbt_cond_signal(cond);\n')
        sim.write('\t\t\t\t\txbt_mutex_release(m);\n')
        sim.write('\t\t\t\t}\n') 
        sim.write('\t\t\t}\n')
        #sim.write('\t\t\t\tnew_task = MSG_task_create("End", 0, 0, NULL);\n')
        #sim.write('\t\t\t\tsprintf(dest, "fin-0");\n')
        #sim.write('\t\t\t\tMSG_task_send(new_task, dest);\n\t\t\t}\n')
        sim.write('\t\t}\n\t\telse\n\t\t{\n')
        sim.write('\t\t\tif(num_event_exts == events)\n\t\t\t{\n')
        sim.write('\t\t\t\txbt_mutex_acquire(m);\n')
        sim.write('\t\t\t\txbt_cond_signal(cond);\n')
        sim.write('\t\t\t\txbt_mutex_release(m);\n')
        sim.write('\t\t\t}\n') 
        sim.write('\t\t}\n') 
        #sim.write('\t\t\tnew_task = MSG_task_create("End", 0, 0, NULL);\n')
        #sim.write('\t\t\tsprintf(dest, "fin-0");\n')
        #sim.write('\t\t\tMSG_task_send(new_task, dest);\n\t\t}\n')
        #sim.write('\t\txbt_mutex_acquire(m);\n')
        sim.write('\t\tnum_delt_ints++;\n')
        #sim.write('\t\tif(num_delt_ints % 1000000 == 0) printf("           >>> %ld\\n", num_delt_ints);\n')
        #sim.write('\t\txbt_mutex_release(m);\n')
        sim.write('\t}\n')
        sim.write('\treturn 0;\n}\n\n')


        sim.write('int generator(int argc, char *argv[])\n{\n')
        sim.write('\tchar* dest[30];\n\n')
        sim.write('\tfor(int i = 0; i < DEPTH; i++){\n')
        sim.write('\t\tfor(int j = 0; j < WIDTH-1; j++){\n')
        sim.write('\t\t\tmsg_task_t new_task = NULL;\n')
        sim.write('\t\t\tnew_task = MSG_task_create("0", 0, 0, NULL);\n')
        sim.write('\t\t\tsprintf(dest, "cp-%d-%d", i, j);\n')
        sim.write('\t\t\tMSG_task_send(new_task, dest);\n')
        sim.write('\t\t\tif( i == DEPTH - 1 )break;\n')
        sim.write('\t\t}\n')
        sim.write('\t}\n')
        sim.write('\treturn 0;\n}\n\n')


        sim.write('int finisher(int argc, char *argv[])\n{\n')
        sim.write('\tconst char* dest[30];\n')
        #sim.write('\tsprintf(hostN, "%s", argv[0]);\n')
        sim.write('\tlong events = ((WIDTH * WIDTH - WIDTH) / 2) * (DEPTH - 1) + 1;\n\n')
        sim.write('\txbt_mutex_acquire(m);\n')
        sim.write('\twhile(num_event_exts != events)\n\t{\n')
        sim.write('\t\txbt_cond_wait(cond, m);\n\t}\n')
        sim.write('\txbt_mutex_release(m);\n\n')
        sim.write('\tfor(int i = 0; i < DEPTH; i++){\n')
        sim.write('\t\tfor(int j = 0; j < WIDTH-1; j++){\n')
        sim.write('\t\t\tmsg_task_t new_task = NULL;\n')
        sim.write('\t\t\tnew_task = MSG_task_create("End", 0, 0, NULL);\n')
        sim.write('\t\t\tsprintf(dest, "cp-%d-%d", i, j);\n')
        sim.write('\t\t\tMSG_task_send(new_task, dest);\n')
        sim.write('\t\t\tif( i == DEPTH - 1 )break;\n')
        sim.write('\t\t}\n')
        sim.write('\t}\n')
        sim.write('\treturn 0;\n}\n\n')

    with open("sim.h", "w+") as simh:
        simh.write('#include <string.h>\n')
        simh.write('#include <math.h>\n')
        simh.write('#include <stdio.h>\n')
        simh.write('#include <stdlib.h>\n')
        simh.write('#include "simgrid/plugins/energy.h"\n')
        simh.write('#include "simgrid/msg.h"\n')
        simh.write('#include "xbt/synchro.h"\n\n')

        simh.write('int atomic(int argc, char *argv[]);\n')
        simh.write('int finisher(int argc, char *argv[]);\n')
        simh.write('int generator(int argc, char *argv[]);\n')
        simh.write('void creating_test(char *file);\n')

def simulate():
    os.system('make -s')
    os.system('./main platform.xml')

if __name__ == "__main__":
    main()
    simulate()


'''
        sim.write('\t\tif (element != 0)\n\t\t{\n')
        sim.write('\t\t\tmsg_task_t task2 = NULL;\n')
        sim.write('\t\t\tres = MSG_task_receive(&(task2), MSG_host_get_name(MSG_host_self()));\n')
        sim.write('\t\t\tif (res != MSG_OK) break;\n')
        sim.write('\t\t\tMSG_task_destroy(task2);\n')
        sim.write('\t\t\ttask2 = NULL;\n')
        sim.write('\t\t}\n')
'''