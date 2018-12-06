cd src/HL/
make
mv ClusterHL ../../
cd ../../
gcc -o asizecompacter src/other/asizecompacter.c
g++ -O3 -o Dimension src/other/dimension.cpp
g++ -O3 -o Cluster src/DLA/Cluster.cpp
g++ -o imager src/imagedraw/example.cpp
gcc -o DistArray src/other/builddistarray.c
gcc -o Energies src/other/permutationtest.c

