#! /bin/bash

for i in $(seq 1 7); do
    A="data""$i""a"
    if [[ ! -f tests/$A ]]; then
        echo "File doesn't exists"
        DL="http://users-cs.au.dk/gudmund/randomF14/multisets/""$A"
        wget "$DL" -O tests/$A
    fi
    B="data""$i""b"
    if [[ ! -f tests/$B ]]; then
        echo "File doesn't exists"
        DL="http://users-cs.au.dk/gudmund/randomF14/multisets/""$B"
        wget "$DL" -O tests/$B
    fi
d
done


# RUN THE PROGRAM HERE 
