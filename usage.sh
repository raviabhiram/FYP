#!/bin/bash     
echo CPU: `top -b -n1 | grep "Cpu(s)" | awk '{print $2 + $4}'` >> cpu.xg;
FREE_DATA=`free -m | grep Mem` 
CURRENT=`echo $FREE_DATA | cut -f3 -d' '`
TOTAL=`echo $FREE_DATA | cut -f2 -d' '`
echo RAM: $(echo "scale = 2; $CURRENT/$TOTAL*100" | bc) >> memory.xg;