<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
# Drifttube
A software for analyzing signals of a drift tube detector for charged particles. It then calculates, stores and plots things like the drift time spectrum, the rt-relation, efficiency and afterhit-probablity.
Its electronic signals are first amplified in any way and then recorded by a FADC or any other digitization device. The digitization needs to be converted to a binary file format, this software can read as input. The documentation for this binary file can be found at the wiki for this project.


## Purpose

## Requirements
To fully build this software, including its unit tests you will need the [googletest framework](https://github.com/google/googletest "googletest on github"). However, it would also be possible to edit the makefile to not build test cases. When you have a system wide installation of googletest on your system you can ignore anything related to googletest in the build instructions for both gtest itself as well as the drifttube software.

### googletest build instructions
For a full build instruction follow the build instructions given on it's github page. Following here, short instructions are given that are valid for Linux with cmake installed (tested Oct. 2019) without any explanations.

1. Change to directory where you want to the googletest directory to show up (e.g a subdirectory called "programs" from your home directory `cd programs`
2. Clone googletest (from now on gtest)`git clone https://github.com/google/googletest.git`
3. Create a build directory for gtest `mkdir gtest`
4. Change to new directory `cd gtest`
5. Let cmake create build files `cmake ../googletest/googletest`
6. Build `make`

Last, let's set environment variables used when building the Drifttube software:
```
GTEST_BUILD=`pwd`
cd ../googletest/googletest
GTEST_SRC=`pwd`
```

## Drifttube build instructions
1. Clone the Drifttube repository to whereever you want it to be built `git clone https://github.com/StBies/Drifttube.git`
2. Change to the new directory `cd Drifttube`
3. If not installed system wide, copy or link the built libs as well as the headers from googletest to the Drifttube directory via:
```
mkdir gtest
cd gtest
ln -s ${GTEST_BUILD}/lib lib
ln -s ${GTEST_SRC}/include include
cd ..
```
4. Ready to build, I suggest to not use the zero suppression if not absolutely needed `make DEFINES=NONE`

## Usage
