#!/bin/bash
#SBATCH --job-name termproject
#SBATCH -q secondary
#SBATCH -N 1
#SBATCH --ntasks-per-node=8
#SBATCH --mem=12G
#SBATCH --constraint=amd
#SBATCH --mail-type=ALL
#SBATCH --mail-user=eh2731@wayne.edu
#SBATCH -o output_%j.out
#SBATCH -e errors_%j.err
#SBATCH -t 1:00:00


module load openmpi3/3.1.0
mpirun -np 8 --mca btl ^openib ./squicksort
