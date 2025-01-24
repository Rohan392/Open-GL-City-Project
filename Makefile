CC=g++
CFLAGS=-Iinclude -std=c++14 -g
LIBS=-lfreeglut -lglew32 -lopengl32 -lglu32
# Default target executed when no arguments are given to make.
default_target: main
.PHONY : default_target

main: main.o camera.o building.o InitShader.o car.o light.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

main.o: main.cc 
	$(CC) $(CFLAGS) -c main.cc

building.o: building.cc 
	$(CC) $(CFLAGS) -c building.cc


camera.o: camera.cc 
	$(CC) $(CFLAGS) -c camera.cc
	
# Compile car.o from car.cc
car.o: car.cc
	$(CC) $(CFLAGS) -c $<

light.o: light.cc
	$(CC) $(CFLAGS) -c $<
	
main.o:  camera.h building.h car.h
camera.o: camera.h
building.o: building.h 
car.o: car.h
light.o: light.h
InitShader.o: common/InitShader.cc
	$(CC) $(CFLAGS) -c $^


clean:
	rm -f main *~ *.o
