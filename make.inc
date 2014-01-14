export
CFLAGS=-I./ -I./utils 
LFLAGS= -lfftw3 -lm -ffast-math -O3
CPP=g++
LD=g++
LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

