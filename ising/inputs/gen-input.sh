#!/bin/bash

file_name="input.in"

cat <<EOF > "$file_name"
20
3000000
1000000
1
10.0
1.0
restfile.bin
ergfile.bin
magfile.bin
accepted.out
EOF

prefix="input-t"

for i in `seq 0.1 0.1 5.0`;
    do echo "$i"; 
    sed "5s/.*/$i/" input.in > $prefix-$i.in; 
done
