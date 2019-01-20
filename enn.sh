#!/bin/bash

fprefixes=(P2HLN100000S P1EXACTN100000S P2MCN100000S DLAMC0N DLAMC1N)
sizes=(3000 500 500 16000 5000)
fprefixes2=(FDLA NR5 NR7)
sizes2=(3000 5000 3000)
i=$1
for (( j=0; j<3; j++ ))
do
    time nohup nice -19 ./Energies -p ${fprefixes[i]} -q ${fprefixes2[j]} -1 ${sizes[i]} -2 ${sizes2[j]} -c 99999 &
done
