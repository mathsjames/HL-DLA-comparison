#!/bin/bash

fprefixes=(P2HLN100000S P1EXACTN100000S P2MCN100000S DLAMC1N FDLA NR5 NR7)
sizes=(3000 500 500 5000 3000 5000 3000)
for (( i=0; i<7; i++ ))
do
    nohup nice -19 ./Energies -p ${fprefixes[i]} -q P2HLN10000S -1 ${sizes[i]} -2 3000 -c 99999 &
done
