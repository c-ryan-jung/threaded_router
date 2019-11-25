#!/bin/bash
#Author: Ryan Jung

#SBATCH --time=24:00:00

#SBATCH --nodes=1
## --ntasks-per-node=40
#SBATCH --exclusive

##SBATCH --mem=30464
##SBATCH --mem-per-cpu=1904

## Current work directory. Default is current directory.
##SBATCH --workdir=/path/to/directory

# Partition to use:
#SBATCH -p bii

# export SLURMPARTITION="bii"
# export SLURMACCOUNT="biocomplexity"


# Account code:
#SBATCH -A biocomplexity

# Note: ROUTER_BINARY must be set.

python $ROUTER_BINARY $initialID $nProcessesForNode $tripFilePattern $routeFilePattern $routeLinkFilename $routeNodeFilename $routerBinary $nfaFilename >> $initialID

exit;