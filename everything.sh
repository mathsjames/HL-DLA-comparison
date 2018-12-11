# compile all code
./getcode.sh

# generate actual size data for circular particle for later monte carlo using first 10 seeds
./runHL.sh 1 10 $cores 1 2 1
# take only 1 in every 10 samples and order them
./ASizeCompacter
