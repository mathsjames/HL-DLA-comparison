i=$1
while [ "$i" -le "$2" ]; do
  ./ClusterDLA -p $3 -c "location/locationDLAMC"$4"N"$i -s $i -r $4
  i=$(($i + 1))
done
