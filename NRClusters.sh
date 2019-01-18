i=$1
log=5
if [ $3 -gt 1000000 ]
then
   log=7
fi
while [ "$i" -le "$2" ]; do
  ./FastDLA $3 g "edf/edfNR"$log$i  $i d $4
  i=$(($i + 1))
done
