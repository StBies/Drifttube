#!/bin/bash
# A simple shell script, that reads all data files beginning with Ar in the data directory and executes the program for each one of those files
# Author: Stefan, 10/19/2016.

cd data
ls Ar*.root > rawfiles.dat
cd ..

while read line
do
	./prog.out if=data/$line
done <data/rawfiles.dat
