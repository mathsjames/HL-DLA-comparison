top=$(($1-1))
i=1
r=$4
p=$5
x=$6
g=$7
while [ "$i" -le "$3" ]; do
  bottom=$(($top+1))
  top=$(($1+(($2-$1)*$i)/$3))
  nohup nice -19 taskset -c 0-140 HLClusters.sh $bottom $top $g $r $p $x 1>/dev/null 2>/dev/null &
  i=$(($i+1))
done
