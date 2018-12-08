IFS='
'
wr=images/
eps="E1C"
for x in `ls ./location | grep P2S | grep N100000S1$`; do
  if [ ! -f $wr$x$eps ]; then
    Imager -r location/$x -w $wr$x$eps -p 100000 -e 1 -s 1000 -c 1
  fi
done
