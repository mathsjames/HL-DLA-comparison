cd src/HL/
make
mv ClusterHL ../../
cd ../../
gcc -o ASizeCompacter src/other/asizecompacter.c
g++ -o ZeroAppender src/HL/zeroappender.cpp
g++ -O3 -o EDF src/other/edf.cpp
g++ -O3 -o ClusterDLA src/DLA/clusterDLA.cpp
g++ -o Imager src/imagedraw/imager.cpp
gcc -o DistArray src/other/distarray.c -lm
gcc -O3 -o Energies src/other/energies.c
gcc -o Summarize src/other/summarize.c -lm
cd fastDLA/
./compile
mv fastDLA ../FastDLA
cd ..
