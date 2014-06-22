export
CPP=clang
#CPP=gcc
#CPP=icc
#CPP=NERSC
ifeq ($(CPP), gcc)
FASTMATH=-ffast-math -fstrict-aliasing -fomit-frame-pointer
CFLAGS=-I../ -I./utils -std=c++11 -DGCC -DNDEBUG -m64 -O3 -Wall -Wno-strict-aliasing -pipe
LFLAGS= -L/usr/local/lib  -L~/lib-src/gfortran64/lib/ -lstdc++ -O3 -lacml -lm -m64 -pipe -$(FASTMATH)
endif
ifeq ($(CPP), clang)
#clang generates significantly worse code than gcc for this project, but compiles faster
#so might as well make it a default debug build
FASTMATH=
CFLAGS=-I../ -I./utils -stdlib=libc++ -std=c++11 -g -DCLANG  -DDEBUG -m64 -Werror -W -Wall
LFLAGS= -L/usr/local/lib -lstdc++ -lacml -lm 
endif
ifeq ($(CPP), icc)
export FASTMATH= -ansi-alias -fargument-noalias -fno-alias -no-ansi-alias-check -complex-limited-range -scalar-rep -no-prec-div -finline-functions
export CFLAGS= -no-ipo -std=c++11 -fast -restrict -O3 -fpermissive -DICC -mkl=sequential
export LD=$(CPP)
export LFLAGS= -lstdc++ -lacml
endif
ifeq ($(CPP), NERSC)
FASTMATH=-ffast-math -fstrict-aliasing -fomit-frame-pointer -funroll-loops
CPP=CC
CFLAGS=-I../ -I./utils -std=c++11 -DGCC -DNDEBUG -m64 -Ofast -march=native -mtune=native -pipe
LFLAGS= -L/usr/local/lib -Ofast -march=native -m64  $(FASTMATH)
endif
LD=$(CPP)


LIB_MAIN=../../lib
MAIN_PATH=..
INLINE_HEADERS=../utils/*.hpp ../types/*.hpp
INC_PATH= -I$(MAIN_PATH) -I$(MAIN_PATH)/utils/
