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

# append zero points to the HL Clusters
./runZA.sh 11 3010 P2HLN100000S
./runZA.sh 11 510 P1EXACTN100000S
./runZA.sh 11 510 P2MCN100000S

# generate empirical density functions for distance distribution
./runEDF.sh 11 3010 6 P2HLN100000S
./runEDF.sh 11 510 1 P1EXACTN100000S
./runEDF.sh 11 510 1 P2MCN100000S
./runEDF.sh 11 16010 32 DLAMC0N
./runEDF.sh 11 5010 10 DLAMC1N

# generate distance arrays
fprefixes=(P2HLN100000S P1EXACTN100000S P2MCN100000S DLAMC0N DLAMC1N)
sizes=(3000 500 500 16000 5000)
for (( i=0; i<5; i++ ))
do
    ./runDA.sh ${fprefixes[i]} ${fprefixes[i]} ${sizes[i]} ${sizes[i]} 1
    for (( j=$(( $i + 1 )); j<5; j++ ))
    do
	./runDA.sh ${fprefixes[i]} ${fprefixes[j]} ${sizes[i]} ${sizes[j]} 0
    done
done

# Find energies of permutations
for (( i=0; i<5; i++ ))
do
    for (( j=$(( $i + 1 )); j<5; j++ ))
    do
	time nohup nice -19 ./Energies -p ${fprefixes[i]} -q ${fprefixes[j]} -1 ${sizes[i]} -2 ${sizes[j]} -c 99999 &
    done
done

# generate empirical distribution functions for much larger DLA clusters
./runFDLA.sh 11 3010 75

# generate dist arrays involving larger clusters
./runDA.sh FDLA FDLA 3000 3000 1
for (( i=0; i<5; i++ ))
do
    ./runDA.sh ${fprefixes[i]} FDLA ${sizes[i]} 3000 0
done

# Find energies involving larger Clusters
for (( i=0; i<5; i++ ))
do
	time nohup nice -19 ./Energies -p ${fprefixes[i]} -q FDLA -1 ${sizes[i]} -2 3000 -c 99999 &
done
