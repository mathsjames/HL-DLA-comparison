i=$1
while [ "$i" -le "$2" ]; do
    #  ./FastDLA $3 "location/locationDLAMC"$4"N"$i -s $i -r $4
    # The above line should save distarrays
  i=$(($i + 1))
done
