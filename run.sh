#!/bin/bash

cd data
ls Ar*.root > rawfiles.dat
cd ..

while read line
do
	./prog.out if=data/$line
done <data/rawfiles.dat
