#!/bin/bash

#SBATCH --job-name=HPC_scan
#SBATCH --nodes=4
#SBATCH --cpus-per-task=1
#SBATCH --tasks-per-node=1
#SBATCH --mem=2GB 
#SBATCH --time=01:00:00 
#SBATCH --verbose

module purge 
module load  amber/openmpi/intel/20.06 

echo "Problem 1: pingpong"
mpiexec -n 4 ./pingpong 0 1
### mpiexec -n 8 --oversubscribe ./pingpong 0 1
echo "Problem 3: scan function in mpi"
mpiexec -n 4 ./mpi-scan
