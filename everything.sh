# compile all code
./getcode.sh

# generate actual size data for circular particle for later monte carlo using first 10 seeds
./runHL.sh 1 10 $cores 1 2 1
# take only 1 in every 10 samples and order them
./ASizeCompacter


# simulate the clusters
./runHL.sh 1 3000 100 1 2 0
./runHL.sh 1 500 100 2 1 0
./runHL.sh 1 500 100 3 2 0
./runDLA.sh 1 16000 10 0
./runDLA.sh 1 5000 10 1
