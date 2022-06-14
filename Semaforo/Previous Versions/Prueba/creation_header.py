import json


def main():

    with open('./Input/trafficlights.json') as f:
        data = json.load(f)

        header = open("./Simulador/simulation_devs.h", "w+")

        header.write("#include <string.h>\n")
        header.write("#include <math.h>\n")
        header.write("#include <stdio.h>\n")
        header.write("#include <stdlib.h>\n")
        header.write('#include "simgrid/plugins/energy.h"\n')
        header.write('#include "simgrid/msg.h"\n')
        header.write('#include "xbt/synchro.h"\n\n')



        for j in data['atomicType']['TimeAdvance']['ta']:
            header.write('#define DELTA_' + str(j['state']).upper() + ' ' + str(j['Timeout']) + '\n')

        header.write('\n\n')

        for i in data['atomicType']['states']['state']:
            header.write('int ' + i + '(int argc, char *argv[]);\n')

        header.write('int controller(int argc, char *argv[]);\n')
        header.write('int externalTransition(int argc, char *argv[]);\n')

        header.close()

if __name__ == "__main__":
    main()

'''
        print(len(data['atomicType']['deltint']['InternalTransition']))

        # Estados Totales

        print(data['atomicType']['states']['state'])

        # Primer Estado

        for i in data['atomicType']['deltint']['InternalTransition']:
            if i['-intTransitionID'] == '1':
                print(i['transition']['StartState'])
                break
'''



