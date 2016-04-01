#!/bin/sh
for i in {1..50}
do
	echo "$i" > num.txt
	ns aodv1.tcl < echo num.txt;
	awk -f aodv.awk aodv1.tr;
done