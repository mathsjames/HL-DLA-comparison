cd src/HL/
make
mv ClusterHL ../../
cd ../../
gcc -o ASizeCompacter src/other/asizecompacter.c
g++ -O3 -o EDF src/other/edf.cpp
g++ -O3 -o ClusterDLA src/DLA/clusterDLA.cpp
g++ -o Imager src/imagedraw/imager.cpp
gcc -o DistArray src/other/distarray.c -lm
gcc -O3 -o Energies src/other/energies.c
