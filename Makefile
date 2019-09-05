#
#  makefile for sb
#  this is a straight forward checking of dependencies
#

#   mol_comp_template requires you to set which program is being compiled
#  lancelot uses Clemente functions, lancelot_gto uses Gaussian functions

.DEFAULT_GOAL := all


# these for linux
#CC = g++ 
#CC = icpc
CC = pgc++
#
# GCC compile flags to consider
#   -O3 is old standard, but docs now recommen -O2
#   -msse2  -mavx256  -march=haswell
#COMPILE_OPTIONS = -O3 -static -m64 -march=haswell -Wall
#COMPILE_OPTIONS = -L/usr/lib/x86_64-redhat-linux6E/lib64 -O3 -static -m64 -march=skylake-avx512 -Wall
#COMPILE_OPTIONS = -O3 -m64 -march=skylake-avx512 -Wall
#COMPILE_OPTIONS = -O3 -m64 -march=ivybridge -mtune=ivybridge -Wall
#COMPILE_OPTIONS = -O3 -m64 -Wall
#  GCC OpenMP parallelization must be dynamic linked
#COMPILE_OPTIONS = -O3 -m64 -march=skylake-avx512 -Wall -fopenmp
#  dynamic linked takes twice as long to run, but is necessary for openmp
#
#  Intel compiler flags   -w3 -Wremarks gives usually useless warnings
#  There are reports of -O3 -qopenmp giving wrong results intel version 12
#COMPILE_OPTIONS = -O3 -m64 -march=skylake-avx512 -Wall -qopenmp
#COMPILE_OPTIONS = -O3 -m64 -march=skylake-avx512 -Wall
#COMPILE_OPTIONS = -O2 -m64 -Wall -qopenmp
#COMPILE_OPTIONS = -O3 -m64 -march=skylake-avx512 -Wall -ipo
#
#  PGI compile flags
COMPILE_OPTIONS = -O4 -m64 -tp=skylake -Wall -Mnoopenmp
#COMPILE_OPTIONS = -O4 -m64 -tp=skylake -Wall -mp

# These for Mac
#  newer version from mac ports
#CC = g++-mp-4.7  # older OSX versions 10.9 or before
#CC = g++-mp-6
#CC = g++-mp-7
#CC = g++  # a sym link to clang++
#CC = pgc++   # for PGI community edition
#COMPILE_OPTIONS = -O3 -static -m64 -msse2 -Wall

# these for sun
#CC = CC 
#COMPILE_OPTIONS = -O3 

EXE_NAME = sb 
OBJS = 
LIBS = -lm

all : $(EXE_NAME)

sb : sb.o
	$(CC) sb.o $(OBJS) $(COMPILE_OPTIONS) -o sb $(LIBS)

sb.o : sb.cc sb.h 
	$(CC) $(COMPILE_OPTIONS) -c sb.cc $(LIBS)


clean:
	rm -f *.o sb 

