fprefix=$4
i=1
while [ "$i" -le "$3" ]; do
  bottom=$(($top+1))
  top=$(($1+(($2-$1)*$i)/$3))
  time nohup nice -19 taskset -c 0-140 EDFthread.sh $bottom $top 100000 $fprefix &
  i=$(($i+1))
done
