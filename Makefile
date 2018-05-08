C=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -std=c++11 -g
LDFLAGS=
LDLIBS= -lm -lGL -lX11

SRCS=main.cpp GameObject.cpp Graphics.cpp Simulator.cpp Street.cpp Vehicle.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: wielo
debug: CPPFLAGS+=-g
debug: LDFLAGS+=-g
debug: traffic

wielo: $(OBJS)
	$(CXX) $(LDFLAGS)  $(OBJS) -o traffic  $(LDLIBS) 

main.o: main.cpp

GameObject.o: GameObject.cpp

Graphics.o: Graphics.cpp

Simulator.o: Simulator.cpp

Street.o: Street.cpp

Vehicle.o: Vehicle.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) traffic
