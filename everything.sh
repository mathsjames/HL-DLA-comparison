# compile all code
./getcode.sh

# generate actual size data for circular particle for later monte carlo using first 10 seeds
./runHL.sh 1 10 10 1 2 1
# take only 1 in every 10 samples and order them
./ASizeCompacter


# simulate the clusters
./runHL.sh 1 3010 100 1 2 0
./runHL.sh 1 510 100 2 1 0
./runHL.sh 1 510 100 3 2 0
./runDLA.sh 1 16010 10 0
./runDLA.sh 1 5010 10 1

# generate empirical density functions for distance distribution
./runEDF.sh 11 3010 6 P2HLN100000S
./runEDF.sh 11 510 1 P2EXACTN100000S
./runEDF.sh 11 510 1 P2MCN100000S
./runEDF.sh 11 16010 32 DLAMC0N
./runEDF.sh 11 5010 10 DLAMC1N

# generate distance arrays
fprefixes=(P2HLN100000S P2EXACTN100000S P2MCN100000S DLAMC0N DLAMC1N)
sizes=(3000 500 500 16000 5000)
for i in {0..4}
do
    ./runDA.sh ${fprefixes[i]} ${fprefixes[i]} ${sizes[i]} ${sizes[i]} 1
    for j in {$(($i + 1))..4}
    do
	./runDA.sh ${fprefixes[i]} ${fprefixes[j]} ${sizes[i]} ${sizes[j]} 0
    done
done

# Find energies of permutations
for i in {0..4}
do
    for j in {$(($i + 1))..4}
    do
	time nohup nice -19 ./Energies -p ${fprefixes[i]} -q ${fprefixes[j]} -1 ${sizes[i]} -2 ${sizes[j]} -c 100000 &
    done
done
