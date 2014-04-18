export
CPP=clang
CPP=gcc
#CPP=icc
ifeq ($(CPP), gcc)
FASTMATH=-funroll-loops -ffast-math -fstrict-aliasing -fomit-frame-pointer -funsafe-loop-optimizations 
CFLAGS=-I../ -I./utils -std=c++11 -DGCC -DNDEBUG -m64 -O3 -march=native -Wall -Werror
LD=gcc
LFLAGS= -L/usr/local/lib -lstdc++ -O3 -lfftw3 -lm -march=native -m64 $(FASTMATH)
endif
ifeq ($(CPP), clang)
#clang generates significantly worse code than gcc for this project, but compiles faster
#so might as well make it a default debug build
FASTMATH=
CFLAGS=-I../ -I./utils -std=c++11 -g -DCLANG  -DDEBUG -m64 -Werror -W -Wall
LD=clang
LFLAGS= -L/usr/local/lib -lstdc++ -lfftw3 -lm 
endif
ifeq ($(CPP), CC)
export FAST_FLAGS= -ansi-alias -fargument-noalias -flto -fno-alias -funroll-loops -no-ansi-alias-check -complex-limited-range -scalar-rep -no-prec-div -finline-functions
export CFLAGS= -fast -no-ipo -restrict -O3 -flto -fpermissive -DICC -mkl=sequential
export LD=CC
export LFLAGS= -lfftw3 -mkl=sequential
endif

LIB_MAIN=../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.hpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/
