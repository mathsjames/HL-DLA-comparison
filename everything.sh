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

# generate empirical distribution functions for much larger DLA clusters and noise reduce DLA clusters
./runFDLA.sh 11 3010 75
./runNRDLA.sh 11 5010 3 10000 0.03
./runNRDLA.sh 11 3010 20 10000000 0.03

# generate dist arrays involving extra clusters
fprefixes2=(FDLA NR5 NR7)
sizes2=(3000 5000 3000)
for (( i=0; i<3; i++ ))
do
    ./runDA.sh ${fprefixes2[i]} ${fprefixes2[i]} ${sizes2[i]} ${sizes2[i]} 1
    for (( j=$(( $i + 1 )); j<3; j++ ))
    do
	./runDA.sh ${fprefixes2[i]} ${fprefixes2[j]} ${sizes2[i]} ${sizes2[j]} 0
    done
done

for (( i=0; i<5; i++ ))
do
    for (( j=0; j<3; j++ ))
    do
    ./runDA.sh ${fprefixes[i]} ${fprefixes2[j]} ${sizes[i]} ${sizes2[j]} 0
    done
done

# Find energies involving extra Clusters
for (( i=0; i<3; i++ ))
do
    for (( j=$(( $i + 1 )); j<3; j++ ))
    do
	time nohup nice -19 ./Energies -p ${fprefixes2[i]} -q ${fprefixes2[j]} -1 ${sizes2[i]} -2 ${sizes2[j]} -c 99999 &
    done
done

for (( i=0; i<5; i++ ))
do
    for (( j=0; j<3; j++ ))
    do
	time nohup nice -19 ./Energies -p ${fprefixes[i]} -q ${fprefixes2[j]} -1 ${sizes[i]} -2 ${sizes2[j]} -c 99999 &
    done
done
