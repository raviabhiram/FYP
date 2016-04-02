#!/bin/sh
for i in {1..50}
do
	echo "$i" > num.txt;
	echo "$i" >> aodv_throughput.xg;
	echo "$i" >> aodv_delay.xg;
	ns aodv1.tcl < num.txt;
	if [ $? -eq 0 ]
	then
		awk -f aodv.awk aodv1.tr;
	fi
done