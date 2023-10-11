#!/bin/bash

file_name="input.in"

cat <<EOF > "$file_name"
20
3000000
100
10.0
1.0
restfile.bin
ergfile.bin
magfile.bin
EOF

prefix="input-t"

for i in `seq 0.1 0.1 5.0`;
    do echo "$i"; 
    sed "4s/.*/$i.0/" input.in > $prefix-$i.in; 
done
