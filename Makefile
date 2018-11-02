C=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -std=c++11 -g
LDFLAGS=
LDLIBS= -lm -lGL -lX11

SRCS=main.cpp EngineCoreBase.cpp EngineCoreWindows.cpp EngineCoreLinux.cpp ObjectsLoader.cpp Vec3.cpp GameObject.cpp Graphics.cpp Simulator.cpp Street.cpp Vehicle.cpp 
OBJS=$(subst .cpp,.o,$(SRCS))

all: wielo
debug: CPPFLAGS+=-g
debug: LDFLAGS+=-g
debug: traffic

wielo: $(OBJS)
	$(CXX) $(LDFLAGS)  $(OBJS) -o traffic  $(LDLIBS) 

main.o: main.cpp

EngineCoreBase.o: EngineCoreBase.cpp
EngineCoreWindows: EngineCoreWindows.cpp
EngineCoreLinux: EngineCoreLinux.cpp
ObjectsLoader: ObjectsLoader.cpp
Vec3: Vec3.cpp
GameObject.o: GameObject.cpp
Graphics.o: Graphics.cpp
Simulator.o: Simulator.cpp
Street.o: Street.cpp
Vehicle.o: Vehicle.cpp



clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) traffic
