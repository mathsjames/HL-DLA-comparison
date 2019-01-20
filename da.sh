#!/bin/bash

fprefixes=(P2HLN100000S P1EXACTN100000S P2MCN100000S DLAMC0N DLAMC1N)
sizes=(3000 500 500 16000 5000)

for (( i=0; i<5; i++ ))
do
    ./runDA.sh ${fprefixes[i]} ${fprefixes[i]} ${sizes[i]} ${sizes[i]} 1
    for (( j=$(( $i + 1 )); j<5; j++ ))
    do
	./runDA.sh ${fprefixes[i]} ${fprefixes[j]} ${sizes[i]} ${sizes[j]} 0
    done
done
