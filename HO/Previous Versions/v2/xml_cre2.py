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

    link = xml.SubElement(AS, 'link')
    link.set('id', 'linkGen0Coupled0')
    link.set('latency', '0')
    link.set('bandwidth', '1Gbps')

    for i in range(depth):
        link = xml.SubElement(AS, 'link')
        idl = "linkCoupled" + str(i) + "Fin0"
        link.set('id', idl)
        link.set('latency', '0')
        link.set('bandwidth', '1Gbps')

    for i in range(depth - 1):
        link = xml.SubElement(AS, 'link')
        idl = "linkCoupled" + str(i) + "Coupled" + str(i+1)
        link.set('id', idl)
        link.set('latency', '0')
        link.set('bandwidth', '1Gbps')
    
    ASroute = xml.SubElement(AS, 'ASroute')
    ASroute.set('src', 'Generator0')
    ASroute.set('dst', 'Coupled-0')
    ASroute.set('gw_src', 'RGenerator0')
    ASroute.set('gw_dst', "RCoupled0")

    link_ctn = xml.SubElement(ASroute, 'link_ctn')
    link_ctn.set('id', "linkGen0Coupled0")

    for i in range(depth):
        ASroute = xml.SubElement(AS, 'ASroute')
        ASroute.set('src', "Coupled-" + str(i))
        ASroute.set('dst', 'Finisher0')
        ASroute.set('gw_src', "RCoupled" + str(i))
        ASroute.set('gw_dst', 'RFinisher0')

        link_ctn = xml.SubElement(ASroute, 'link_ctn')
        link_ctn.set('id', "linkCoupled" + str(i) + "Fin0")

    for i in range(depth - 1):
        ASroute = xml.SubElement(AS, 'ASroute')
        ASroute.set('src', "Coupled-" + str(i))
        ASroute.set('dst', "Coupled-" + str(i+1))
        ASroute.set('gw_src', "RCoupled" + str(i))
        ASroute.set('gw_dst', "RCoupled" + str(i+1))

        link_ctn = xml.SubElement(ASroute, 'link_ctn')
        link_ctn.set('id', "linkCoupled" + str(i) + "Coupled" + str(i+1))

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

if __name__ == "__main__":
    main()