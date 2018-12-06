

i=$1
while [ "$i" -le "$2" ]; do
  Cluster -p $3 -c "cluster/Cluster"$i -l /dev/null -s $i
  i=$(($i + 1))
done