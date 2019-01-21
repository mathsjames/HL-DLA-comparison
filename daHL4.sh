#!/bin/bash

fprefixes=(P2HLN100000S P1EXACTN100000S P2MCN100000S DLAMC1N FDLA NR5 NR7)
sizes=(3000 500 500 5000 3000 5000 3000)
./runDA.sh P2HLN10000S P2HLN10000S 3000 3000 1
for (( i=0; i<7; i++ ))
do
    ./runDA.sh ${fprefixes[i]} P2HLN10000S ${sizes[i]} 3000 0
done
