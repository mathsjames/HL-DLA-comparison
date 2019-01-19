#!/bin/bash

fprefixes2=(FDLA NR5 NR7)
sizes2=(3000 5000 3000)
for (( i=0; i<3; i++ ))
do
    for (( j=$(( $i + 1 )); j<3; j++ ))
    do
		time nohup nice -19 ./Energies -p ${fprefixes2[i]} -q ${fprefixes2[j]} -1 ${sizes2[i]} -2 ${sizes2[j]} -c 99999 &
    done
done
