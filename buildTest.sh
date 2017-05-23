#!/bin/bash

g++ -std=c++11 -I./gtest/include -L./gtest/lib src/unitTests/EventTest.cpp obj/Event.o -o EventTest.out -lgtest -lgtest_main -lpthread -fopenmp
g++ -std=c++11 -I./gtest/include -L./gtest/lib src/unitTests/DrifttubeTest.cpp obj/DriftTube.o obj/DataSet.o obj/Event.o obj/Exception.o obj/EventSizeException.o -o DrifttubeTest.out -lgtest -lgtest_main -lpthread -fopenmp
g++ -std=c++11 -I./gtest/include -L./gtest/lib src/unitTests/DataSetTest.cpp obj/DataSet.o obj/Event.o obj/Exception.o obj/EventSizeException.o -o DataSetTest.out -lgtest -lgtest_main -lpthread -fopenmp

