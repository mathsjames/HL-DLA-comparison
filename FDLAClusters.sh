i=$1
while [ "$i" -le "$2" ]; do
  ./FastDLA $3 g "edf/edfFDLA"$i  $i d
  i=$(($i + 1))
done
