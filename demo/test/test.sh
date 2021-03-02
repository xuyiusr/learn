#!/bin/bash

declare -a list
total_used=0
max

IFS=$'\n'
for line in `top -b -n1 | awk '/Cpu|KiB Mem/'`
do
    list[${#list[*]}]=${line}
done

cpu=`echo ${list[0]} | awk '{printf $2}'`
mem=`echo ${list[1]} | awk '{printf $4}'`
