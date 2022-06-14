import csv, sys, os, datetime
import xml.etree.ElementTree as xml
import xml.dom.minidom as xmldom

def main():
	width = int(sys.argv[1])
	depth = int(sys.argv[2])

	platform = xml.Element("platform")
	platform.set('version', '4.1')

	AS = xml.SubElement(platform, 'AS')
	AS.set('id', 'AS0')
	AS.set('routing', 'None')

	for i in range(depth):
		for j in range(width - 1):
			if i == depth - 1:
			    ida = "cp-" + str(i) + "-0"
			else:
				ida = "cp-" + str(i) + "-" + str(j)

			h = xml.SubElement(AS, 'host')
			h.set('id', ida)
			h.set('speed', '1e9f')
			
			if i == depth - 1:
				break

	gen = xml.SubElement(AS, 'host')
	gen.set('id', 'gen-0')
	gen.set('speed', '1e9f')

	fin = xml.SubElement(AS, 'host')
	fin.set('id', 'fin-0')
	fin.set('speed', '1e9f')

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


if __name__ == "__main__":
    main()