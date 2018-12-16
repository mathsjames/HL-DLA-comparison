fprefix1=$1
fprefix2=$2
size1=$3
size2=$4
reflexive=$5
time nohup nice -19 taskset -c 0-140 DistArray -p "edf/edf"fprefix1 -q "edf/edf"fprefix2 -1 $size1 -2 $size2 -c 10000 -l 0.04 -r $reflexive
