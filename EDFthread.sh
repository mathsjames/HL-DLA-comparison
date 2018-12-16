i=$1
while [ "$i" -le "$2" ]; do
  ./EDF -p $3 -s $i -n 10000 -r "location/location"$4$i -w "edf/edf"$4$i
  i=$(($i + 1))
done
