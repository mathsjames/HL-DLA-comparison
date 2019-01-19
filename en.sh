#!/bin/bash

fprefixes=(P2HLN100000S P1EXACTN100000S P2MCN100000S DLAMC0N DLAMC1N)
sizes=(3000 500 500 16000 5000)

for (( i=0; i<5; i++ ))
do
    for (( j=$(( $i + 1 )); j<5; j++ ))
    do
	time nohup nice -19 ./Energies -p ${fprefixes[i]} -q ${fprefixes[j]} -1 ${sizes[i]} -2 ${sizes[j]} -c 99999 &
    done
done
