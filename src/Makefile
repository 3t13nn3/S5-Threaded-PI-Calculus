#ETIENNE (づ｡◕‿‿◕｡)づ PENAULT#

SRC	= $(wildcard *.cpp)
OBJS	= $(SRC:.cpp=.o)
HEADER	= $(wildcard *.hpp) $(wildcard *.h)
OUT		= pi
CC	 	= g++

CFLAGS	= -O3 -Wall -W -pedantic -std=c++17

macLFLAGS	= -lgmp -lgmpxx
linuxLFLAGS	= -pthread -lgmp -lgmpxx

OS := $(shell uname)


all: $(OBJS)
ifeq ($(OS), Darwin)
	$(CC) $(OBJS) -o $(OUT) $(macLFLAGS)
else
	$(CC) $(OBJS) -o $(OUT) $(linuxLFLAGS)
endif


%.o:	%.cpp %.hpp
	$(CC) -c $< $(CFLAGS)

%.o:	%.cpp %.h
	$(CC) -c $< $(CFLAGS)

%.o:	%.cpp
	$(CC) -c $< $(CFLAGS)


clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(OUT) *.txt
