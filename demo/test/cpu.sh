#!/bin/bash

declare -a list
total_used=0
max

IFS=$'\n'
for line in `top -b -n 5 -d 2 | grep %Cpu | awk '{print $2}'`
do
    list[${#list[*]}]=${line}
done

for ((i=0;i<${#list[*]};i++))
do
    used=${list[$i]}
    total_used=`echo | awk -v a=$total_used -v b=$used '{printf "%.1f", a + b}'`
done

total_num=${#list[*]}
average=`echo | awk -v a=$total_used -v b=$total_num '{printf "%.2f", a/b}'`
echo "average :${average}"

