mkdir times
size=$1
time ./ClusterHL -S 1 -r 1 -p 2 -g $size &> times/HL
time ./ClusterHL -S 1 -r 2 -p 1 -g $size &> times/EXACT
time ./ClusterHL -S 1 -r 3 -p 2 -g $size &> times/MC
time ./ClusterDLA -s 1 -r 0 -c /dev/null -p $size &> times/DLA
time ./ClusterDLA -s 1 -r 1 -c /dev/null -p $size &> times/DLAMC
