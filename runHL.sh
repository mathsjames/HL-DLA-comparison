top=$(($1-1))
i=1
r=$4
p=$5
x=$6
while [ "$i" -le "$3" ]; do
  bottom=$(($top+1))
  top=$(($1+(($2-$1)*$i)/$3))
  time nohup nice -19 taskset -c 0-140 HLClusters.sh $bottom $top 100000 $r $p $x &
  i=$(($i+1))
done


