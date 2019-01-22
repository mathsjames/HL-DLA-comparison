# compile all code
./getcode.sh

# generate actual size data for circular particle for later monte carlo using first 10 seeds
./runHL.sh 1 10 10 1 2 1 100000
# take only 1 in every 10 samples and order them
./ASizeCompacter


# simulate the clusters
./runHL.sh 1 3010 100 1 2 0 100000
./runHL.sh 1 510 100 2 1 0 100000
./runHL.sh 1 510 100 3 2 0 100000
./runDLA.sh 1 16010 10 0
./runDLA.sh 1 5010 10 1

# append zero points to the HL Clusters
./runZA.sh 11 3010 P2HLN100000S
./runZA.sh 11 510 P1EXACTN100000S
./runZA.sh 11 510 P2MCN100000S

# generate empirical density functions for distance distribution
./runEDF.sh 11 3010 6 P2HLN100000S 100000
./runEDF.sh 11 510 1 P1EXACTN100000S 100000
./runEDF.sh 11 510 1 P2MCN100000S 100000
./runEDF.sh 11 16010 32 DLAMC0N 100000
./runEDF.sh 11 5010 10 DLAMC1N 100000

# generate distance arrays
./da.sh

# Find energies of permutations
./en.sh

# generate empirical distribution functions for much larger DLA clusters and noise reduce DLA clusters
./runFDLA.sh 11 3010 75
./runNRDLA.sh 11 5010 3 100000 0.03
./runNRDLA.sh 11 3010 20 10000000 0.03

# generate dist arrays involving extra clusters
./dainter.sh
./daintra.sh
./dan.sh 0
./dan.sh 1
./dan.sh 2
./dan.sh 3
./dan.sh 4

# Find energies involving extra Clusters
./eninter.sh
./enn.sh 0
./enn.sh 1
./enn.sh 2
./enn.sh 3
./enn.sh 4

#computations for smaller HL clusters
./runHL.sh 11 3010 4 1 2 0 10000

./runZA.sh 11 3010 P2HLN10000S

./runEDF.sh 11 3010 4 P2HLN10000S 10000

./daHL4.sh
./runDA.sh DLAMC0N P2HLN10000S 16000 3000 0

./enHL4.sh
nohup nice -19 ./Energies -p DLAMC0N -q P2HLN10000S -1 16000 -2 3000 -c 99999 &

#Print results to terminal
./Summarize
