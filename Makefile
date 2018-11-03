C=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -std=c++11 -g
LDFLAGS=
LDLIBS= -lm -lGL -lX11

SRCS=src/main.cpp
SRCS+=src/simulator/EngineCore/EngineCoreBase.cpp 
SRCS+=src/simulator/EngineCore/EngineCoreWindows.cpp
SRCS+=src/simulator/EngineCore/EngineCoreLinux.cpp
SRCS+=src/simulator/EngineCore/Graphics.cpp
SRCS+=src/simulator/EngineCore/Vec3.cpp
SRCS+=src/simulator/EngineCore/Colors.cpp
SRCS+=src/simulator/EngineCore/ExceptionClass.cpp

SRCS+=src/simulator/ObjectsLoader.cpp
SRCS+=src/simulator/GameObject.cpp
SRCS+=src/simulator/Simulator.cpp
SRCS+=src/simulator/Road.cpp
SRCS+=src/simulator/Vehicle.cpp 
SRCS+=src/simulator/Garage.cpp

OBJS=$(subst .cpp,.o,$(SRCS))

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
Street.o: Road.cpp
Vehicle.o: Vehicle.cpp
Garage.o: Garage.cpp
Colors.o: Colors.cpp
ExceptionClass.o: ExceptionClass.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) traffic
