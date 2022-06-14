import csv, sys, os, datetime
import xml.etree.ElementTree as xml
import xml.dom.minidom as xmldom

def main():
    if len(sys.argv) != 3:
        print("python3 " + str(sys.argv[0]) + " <width> <depth>") 
        exit(0)

    width = int(sys.argv[1])
    depth = int(sys.argv[2])

    startXML = datetime.datetime.now()

    platform = xml.Element("platform")
    platform.set('version', '4.1')

    AS = xml.SubElement(platform, 'AS')
    AS.set('id', 'AS0')
    AS.set('routing', 'Full')

    for i in range(depth):
        ida = "Coupled-" + str(i)
        prefix = "cp-" + str(i) + "-"
        rad = "0-" + str(width - 1)

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
        fh.write("<?xml version='1.0'?><!DOCTYPE platform SYSTEM 'http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd'>".encode(
                'utf-8'))

    with open("platform.xml", "ab") as fh:
        tree.write(fh, encoding='utf-8')

    dom = xmldom.parse("platform.xml")
    string = dom.toprettyxml()

    f = open("platform.xml", "w")
    string = string.replace('<?xml version="1.0" ?>','<?xml version="1.0"?>')
    f.write(string)
    f.close()

    endXML = datetime.datetime.now()
    diffXML = (endXML - startXML)
    execXML = round(diffXML.total_seconds(), 5)
    print("Platform creation time: " + str(execXML) + " s")

    startSimg = datetime.datetime.now()

    with open("main.c", "w+") as main:
        main.write('#include "./sim.h"\n')
        main.write('#include <sys/time.h>\n\n')

        main.write('#define WIDTH ' + str(width) + '\n')
        main.write('#define DEPTH ' + str(depth) + '\n\n')


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
        main.write('\t\tfor(int j = 0; j < WIDTH; j++){\n')
        main.write('\t\t\tsprintf(str, "cp-%d-%d", i, j);\n')
        main.write('\t\t\targc = 3;\n')
        main.write('\t\t\tchar **argvc = xbt_new(char *, 4);\n')
        main.write('\t\t\targvc[0] = bprintf("%d", i);\n')
        main.write('\t\t\targvc[1] = bprintf("%d", j);\n')
        main.write('\t\t\targvc[2] = bprintf("%s", str);\n')
        main.write('\t\t\targvc[3] = NULL;\n')
        main.write('\t\t\tp = MSG_process_create_with_arguments(str, atomic, NULL, MSG_get_host_by_name(str), argc, argvc);\n\n')
        main.write('\t\t\tif( p == NULL )\n\t\t\t{\n')
        main.write('\t\t\t\tprintf("Error en %s\\n", str);\n')
        main.write('\t\t\t\texit(0);\n\t\t\t}\n')
        main.write('\t\t}\n\t}\n\n')

        main.write('\tsprintf(str, "gen-0");\n')
        main.write('\targc = 1;\n')
        main.write('\tchar **argvc1 = xbt_new(char *, 4);\n')
        main.write('\targvc1[0] = bprintf("%s", str);\n')
        main.write('\targvc1[1] = NULL;\n')
        main.write('\tp = MSG_process_create_with_arguments(str, generator, NULL, MSG_get_host_by_name(str), argc, argvc1);\n\n')
        main.write('\tif( p == NULL )\n\t{\n')
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
        main.write('\tmsg_error_t res = MSG_OK;\n\n')
        main.write('\tif (argc > 2)\n\t{\n')
        main.write('\t\tprintf("./%s platform_user.xml \\n", argv[0]);\n')
        main.write('\t\texit(1);\n\t}\n\n')
        main.write('\tMSG_init(&argc, argv);\n')
        main.write('\tcreating_test(argv[1]);\n')
        main.write('\tdouble t1 = get_time();\n')
        main.write('\tres = MSG_main();\n')
        main.write('\tdouble t2 = get_time() - t1;\n')
        main.write('\tprintf("Simulation time: %.5f s\\n", t2);\n\n')
        main.write('\tif ( res == MSG_OK) return 0;\n')
        main.write('\telse return 1;\n}\n')

    with open("sim.c", "w+") as sim:
        sim.write('#include "./sim.h"\n\n')

        sim.write('#define WIDTH ' + str(width) + '\n')
        sim.write('#define DEPTH ' + str(depth) + '\n\n')

        sim.write('int atomic(int argc, char *argv[])\n{\n')
        sim.write('\tchar* hostname[40];\n')
        sim.write('\tmsg_task_t task = NULL, new_task = NULL;\n')
        sim.write('\tsprintf(hostname, "%s", argv[2]);\n')
        sim.write('\tint depth = atoi(argv[0]), element = atoi(argv[1]), res = 0;\n\n')
        sim.write('\tMSG_mailbox_set_async(hostname);\n')
        sim.write('\tmsg_host_t host = MSG_host_by_name(hostname);\n\n')
        sim.write('\twhile(1)\n\t{\n')
        sim.write('\t\tres = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));\n')
        sim.write('\t\tif (res != MSG_OK) break;\n\n')
        sim.write('\t\tif (strcmp(MSG_task_get_name(task), "End") == 0)\n\t\t{\n')
        sim.write('\t\t\tMSG_task_destroy(task);\n')
        sim.write('\t\t\ttask = NULL;\n')
        sim.write('\t\t\tbreak;\n\t\t}\n')
        sim.write('\t\telse if(strcmp(MSG_task_get_name(task), "1") == 0)\n\t\t{\n')
        sim.write('\t\t\tMSG_task_destroy(task);\n')
        sim.write('\t\t\ttask = NULL;\n')
        sim.write('\t\t\tint next = (element + 1) % WIDTH;\n')
        sim.write('\t\t\tif (next != 0)\n\t\t\t{\n')
        sim.write('\t\t\t\tnew_task = MSG_task_create("1", 0, 0, NULL);\n')
        sim.write('\t\t\t\tsprintf(hostname, "cp-%d-%d", depth, next);\n')
        sim.write('\t\t\t\tMSG_task_send(new_task, hostname);\n\t\t\t}\n')
        sim.write('\t\t\telse\n\t\t\t{\n')
        sim.write('\t\t\t\tnew_task = MSG_task_create("End", 0, 0, NULL);\n')
        sim.write('\t\t\t\tsprintf(hostname, "fin-0");\n')
        sim.write('\t\t\t\tMSG_task_send(new_task, hostname);\n\t\t\t}\n')
        sim.write('\t\t}\n')
        sim.write('\t}\n')
        sim.write('\treturn 0;\n}\n\n')


        sim.write('int generator(int argc, char *argv[])\n{\n')
        sim.write('\tmsg_task_t task = NULL, new_task = NULL;\n')
        sim.write('\tchar* hostname[40];\n\n')
        sim.write('\tfor(int i = 0; i < DEPTH; i++)\n\t{\n')
        sim.write('\t\tnew_task = MSG_task_create("1", 0, 0, NULL);\n')
        sim.write('\t\tsprintf(hostname, "cp-%d-0", i);\n')
        sim.write('\t\tMSG_task_send(new_task, hostname);\n')
        sim.write('\t}\n')
        sim.write('\treturn 0;\n}\n\n')


        sim.write('int finisher(int argc, char *argv[])\n{\n')
        sim.write('\tconst char* hostname[40];\n')
        sim.write('\tmsg_task_t task = NULL, new_task = NULL;\n')
        sim.write('\tsprintf(hostname, "%s", argv[2]);\n')
        sim.write('\tint total_messages = 0, res = 0;\n\n')
        sim.write('\tMSG_mailbox_set_async(hostname);\n')
        sim.write('\tmsg_host_t host = MSG_host_by_name(hostname);\n\n')
        sim.write('\twhile(1)\n\t{\n')
        sim.write('\t\tres = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));\n')
        sim.write('\t\tif (res != MSG_OK) break;\n\n')
        sim.write('\t\tif (strcmp(MSG_task_get_name(task), "End") == 0)\n\t\t{\n')
        sim.write('\t\t\tMSG_task_destroy(task);\n')
        sim.write('\t\t\ttask = NULL;\n')
        sim.write('\t\t\ttotal_messages++;\n')
        sim.write('\t\t\tif (total_messages == DEPTH)\n\t\t\t{\n')
        sim.write('\t\t\t\tfor (int i = 0; i < DEPTH; i++){\n')
        sim.write('\t\t\t\t\tfor (int j = 0; j < WIDTH; j++){\n')
        sim.write('\t\t\t\t\t\tnew_task = MSG_task_create("End", 0, 0, NULL);\n')
        sim.write('\t\t\t\t\t\tsprintf(hostname, "cp-%d-%d", i, j);\n')
        sim.write('\t\t\t\t\t\tMSG_task_send(new_task, hostname);\n')
        sim.write('\t\t\t\t\t}\n')
        sim.write('\t\t\t\t}\n')
        sim.write('\t\t\t\tbreak;\n')
        sim.write('\t\t\t}\n')
        sim.write('\t\t}\n\t}\n')
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

    endSimg = datetime.datetime.now()
    diffSimg = (endSimg - startSimg)
    execSimg = round(diffSimg.total_seconds(), 5)
    print("Environment creation time: " + str(execSimg) + " s")


def simulate():
    os.system('make -s')
    os.system('./main platform.xml')

if __name__ == "__main__":
    main()
    simulate()
