fprefix=$3
i=$1
while [ "$i" -le "$2" ]; do
  ./ZeroAppender "location/location"$fprefix$i
  i=$(($i + 1))
done
