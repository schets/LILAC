export
CFLAGS=-I./ -I./utils -g -std=c++03 -O3 -ffast-math  -fstrict-aliasing -march=native 
LFLAGS= -lstdc++ -lfftw3 -lgsl -lgslcblas -lm 
CPP=gcc
LD=gcc

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

