#!/bin/bash

inFileSet=0
while [ $inFileSet -lt 1 ]
do
    echo "path to FADC data: "
    read infile

    if [ ! -f $infile ]
    then
        echo "$infile does not exist"
    else
        inFileSet=1
    fi
done

echo $infile > run.conf
