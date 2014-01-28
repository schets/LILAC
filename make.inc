export
CFLAGS=-I./ -I./utils -ffast-math -O3 -fstrict-aliasing -march=native -ftree-vectorizer-verbose=1
LFLAGS= -lstdc++ -lfftw3 -lm 
CPP=gcc
LD=gcc

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.cpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/

