#! /bin/bash

for i in $(seq 1 7); do
    A="data""$i""a"
    if [[ ! -f build/$A ]]; then
        echo "File doesn't exists"
        DL="http://users-cs.au.dk/gudmund/randomF14/multisets/""$A"
        wget "$DL" -O build/$A
    fi
    B="data""$i""b"
    if [[ ! -f build/$B ]]; then
        echo "File doesn't exists"
        DL="http://users-cs.au.dk/gudmund/randomF14/multisets/""$B"
        wget "$DL" -O build/$B
    fi
d
done


# RUN THE PROGRAM HERE 
