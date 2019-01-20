#!/bin/bash

fprefixes2=(FDLA NR5 NR7)
sizes2=(3000 5000 3000)
for (( i=0; i<3; i++ ))
do
    ./runDA.sh ${fprefixes2[i]} ${fprefixes2[i]} ${sizes2[i]} ${sizes2[i]} 1
done
