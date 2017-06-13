#!/bin/bash

g++ -std=c++11 -fopenmp -I./gtest/include -L./gtest/lib src/unitTests/Event_test.cpp obj/Event.o -o EventTest.out -lgtest -lgtest_main -lpthread -fopenmp
g++ -std=c++11 -fopenmp -I./gtest/include -L./gtest/lib src/unitTests/DriftTimeSpectrum_test.cpp obj/DriftTimeSpectrum.o obj/Event.o -o DriftTimeSpectrumTest.out -lgtest -lgtest_main -lpthread -fopenmp
g++ -std=c++11 -fopenmp -I./gtest/include -L./gtest/lib src/unitTests/Drifttube_test.cpp obj/Drifttube.o obj/DataSet.o obj/Event.o obj/Exception.o obj/EventSizeException.o -o DrifttubeTest.out -lgtest -lgtest_main -lpthread -fopenmp
g++ -std=c++11 -fopenmp -I./gtest/include -L./gtest/lib src/unitTests/DataSet_test.cpp obj/DataSet.o obj/Event.o obj/Exception.o obj/EventSizeException.o -o DataSet_test.out -lgtest -lgtest_main -lpthread -fopenmp
g++ -std=c++11 -fopenmp -I./gtest/include -L./gtest/lib src/unitTests/DataProcessor_test.cpp obj/DataProcessor.o obj/Event.o obj/DataSet.o obj/Exception.o obj/EventSizeException.o -o DataProcessorTest.out -lgtest -lgtest_main -lpthread -fopenmp

