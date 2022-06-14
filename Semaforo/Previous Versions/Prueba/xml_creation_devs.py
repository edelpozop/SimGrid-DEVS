import json
import xml.etree.ElementTree as xml
import xml.dom.minidom as xmldom


def main():
    filename = "./Simulador/platform_devs.xml"
    platform = xml.Element("platform")
    platform.set('version', '4.1')

    zone = xml.SubElement(platform, 'zone')
    zone.set('id', 'AS0')
    zone.set('routing', 'Full')

    with open('./Input/trafficlights.json') as f:
        data = json.load(f)

    for j in data['atomicType']['TimeAdvance']['ta']:
        host = xml.SubElement(zone, 'host')
        host.set('id', str(j['state']))
        host.set('speed', '40Gf')

    host = xml.SubElement(zone, 'host')
    host.set('id', 'cont-0')
    host.set('speed', '40Gf')

    host = xml.SubElement(zone, 'host')
    host.set('id', 'externalTransition')
    host.set('speed', '40Gf')

    for i in data['atomicType']['deltint']['InternalTransition']:
        link = xml.SubElement(zone, 'link')
        link.set('id', str(i['transition']['StartState']) + '-' + str(i['transition']['NextState']))
        link.set('bandwidth', '250MBps')
        link.set('latency', '0us')

        route = xml.SubElement(zone, 'route')
        route.set('src', str(i['transition']['StartState']))
        route.set('dst', str(i['transition']['NextState']))

        link_ctn = xml.SubElement(route, 'link_ctn')
        link_ctn.set('id', str(i['transition']['StartState']) + '-' + str(i['transition']['NextState']))


    for i in data['atomicType']['deltext']['ExternalTransition']:
        link = xml.SubElement(zone, 'link')
        link.set('id', str(i['transition']['StartState']) + '-' + str(i['transition']['NextState']))
        link.set('bandwidth', '250MBps')
        link.set('latency', '0us')

        route = xml.SubElement(zone, 'route')
        route.set('src', str(i['transition']['StartState']))
        route.set('dst', str(i['transition']['NextState']))

        link_ctn = xml.SubElement(route, 'link_ctn')
        link_ctn.set('id', str(i['transition']['StartState']) + '-' + str(i['transition']['NextState']))

    for i in data['atomicType']['states']['state']:
        link = xml.SubElement(zone, 'link')
        link.set('id', 'cont-' + i)
        link.set('bandwidth', '250MBps')
        link.set('latency', '0us')

        route = xml.SubElement(zone, 'route')
        route.set('src', 'cont-0')
        route.set('dst', str(i))

        link_ctn = xml.SubElement(route, 'link_ctn')
        link_ctn.set('id', 'cont-' + str(i))

    for i in data['atomicType']['deltext']['ExternalTransition']:
        link = xml.SubElement(zone, 'link')
        link.set('id', 'externalTransition')
        link.set('bandwidth', '250MBps')
        link.set('latency', '0us')

        route = xml.SubElement(zone, 'route')
        route.set('src', 'externalTransition')
        route.set('dst', str(i['transition']['StartState']))

        link_ctn = xml.SubElement(route, 'link_ctn')
        link_ctn.set('id', 'externalTransition-' + str(i['transition']['StartState']))


    tree = xml.ElementTree(platform)

    with open(filename, "wb") as fh:
        fh.write(
            "<?xml version='1.0'?><!DOCTYPE platform SYSTEM 'http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd'>".encode(
                'utf-8'))

    with open(filename, "ab") as fh:
        tree.write(fh, encoding='utf-8')

    dom = xmldom.parse(filename)
    string = dom.toprettyxml()

    f = open(filename, "w")
    f.write(string)
    f.close


if __name__ == "__main__":
    main()