#!/usr/bin/env python
# -*- coding: utf-8 -*-

from xml.etree import ElementTree as ET

tree = ET.parse("./common.xml") 
root = tree.getroot()

config_node = root.getiterator("configuration")
for node in config_node:
    print (node.attrib)
