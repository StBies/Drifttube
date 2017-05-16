#!/bin/bash

g++ -std=c++11 -I./gtest/include -L./gtest/lib src/unitTests/EventTest.cpp obj/Event.o -o EventTest.out -lgtest -lgtest_main -lpthread
