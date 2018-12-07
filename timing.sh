size=$1
{ echo "HL "$size ; time ./ClusterHL -S 1 -r 1 -p 2 -g $size ; } &>> times
{ echo "EXACT "$size ; time ./ClusterHL -S 1 -r 2 -p 1 -g $size ; } &>> times
{ echo "MC "$size ; time ./ClusterHL -S 1 -r 3 -p 2 -g $size ; } &>> times
{ echo "DLA "$size ; time ./ClusterDLA -s 1 -r 0 -c /dev/null -p $size ; } &>> times
{ echo "DLAMC "$size ; time ./ClusterDLA -s 1 -r 1 -c /dev/null -p $size ; } &>> times
