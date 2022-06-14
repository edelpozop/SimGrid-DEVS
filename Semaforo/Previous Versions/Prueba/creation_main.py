import json


def main():

    with open('./Input/trafficlights.json') as f:
        data = json.load(f)

        main = open("./Simulador/main_devs.c", "w+")

        main.write('#include "./Simulador/simulation_devs.h"\n\n')

        main.write("\nvoid test_all(char *file)\n{\n")
        main.write('\tint argc = 0;\n')
        main.write('\tchar str[50];\n')
        main.write('\tmsg_process_t p;\n\n')

        main.write('\tMSG_create_environment(file);\n')
        main.write('\tMSG_function_register("controller", controller);\n')
        main.write('\tMSG_function_register("externalTransition", externalTransition);\n')

        for j in data['atomicType']['TimeAdvance']['ta']:
            main.write('\tMSG_function_register("' + str(j['state']) + '", ' + str(j['state']) + ');\n')

        main.write('\n\n')
        sum = 0

        for j in data['atomicType']['TimeAdvance']['ta']:
            main.write('\n\t/*' + str(j['state']) + '*/\n\n')
            main.write('\tsprintf(str, "' + str(j['state']) + '");\n')
            main.write('\tprintf("Creating %s\\n", str);\n')
            main.write('\tchar **argvc' + str(sum) + ' = xbt_new(char *, 1);\n')
            main.write('\targvc' + str(sum) + '[0] = NULL;\n')
            main.write(
                '\tp = MSG_process_create_with_arguments(str, ' + str(j['state']) + ', NULL, MSG_get_host_by_name(str), argc, argvc' + str(
                    sum) + ');\n\n')
            main.write('\tif( p == NULL )\n\t{\n')
            main.write('\t\tprintf("Error en %s\\n", str);\n')
            main.write('\t\texit(0);\n\t}\n\n')
            sum = sum + 1

        main.write('\n\t/* Controller */\n\n')
        main.write('\tsprintf(str, "controller");\n')
        main.write('\tprintf("Creating %s\\n", str);\n')
        main.write('\tchar **argvc' + str(sum) + ' = xbt_new(char *, 1);\n')
        main.write('\targvc' + str(sum) + '[0] = NULL;\n')
        main.write(
            '\tp = MSG_process_create_with_arguments(str, controller, NULL, MSG_get_host_by_name(str), argc, argvc' + str(
                sum) + ');\n\n')
        main.write('\tif( p == NULL )\n\t{\n')
        main.write('\t\tprintf("Error en %s\\n", str);\n')
        main.write('\t\texit(0);\n\t}\n\n')
        sum = sum + 1

        main.write('\n\t/* externalTransition */\n\n')
        main.write('\tsprintf(str, "externalTransition");\n')
        main.write('\tprintf("Creating %s\\n", str);\n')
        main.write('\tchar **argvc' + str(sum) + ' = xbt_new(char *, 1);\n')
        main.write('\targvc' + str(sum) + '[0] = NULL;\n')
        main.write(
            '\tp = MSG_process_create_with_arguments(str, externalTransition, NULL, MSG_get_host_by_name(str), argc, argvc' + str(
                sum) + ');\n\n')
        main.write('\tif( p == NULL )\n\t{\n')
        main.write('\t\tprintf("Error en %s\\n", str);\n')
        main.write('\t\texit(0);\n\t}\n\n')

        main.write('\treturn;\n\n}')

        main.write('\n\nint main(int argc, char *argv[])\n{\n')
        main.write('\tmsg_error_t res = MSG_OK;\n\n')

        main.write('\tif (argc < 2)\n\t{\n')
        main.write('\t\tprintf("Usage: %s platform_devs.xml", argv[0]);\n')
        main.write('\t\texit(1);\n\t}\n')
        main.write('\tMSG_init(&argc, argv);\n\n')
        main.write('\ttest_all(argv[1]);\n')
        main.write('\tres = MSG_main();\n')

        main.write('\tif (res == MSG_OK) return 0;\n')
        main.write('\telse return 1; \n')
        main.write('}\n')

        main.close()

if __name__ == "__main__":
    main()