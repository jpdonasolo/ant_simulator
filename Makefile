CXFLAGS = -Iheaders -O2 -ljsoncpp -Wall

main: main.o utils.o world.o
	g++ main.o utils.o world.o $(CXFLAGS) -o main

main.o: src/main.cpp
	g++ -c src/main.cpp $(CXFLAGS)

utils.o: src/utils.cpp
	g++ -c src/utils.cpp $(CXFLAGS)

world.o: src/world.cpp
	g++ -c src/world.cpp $(CXFLAGS)

clean:
	rm *.o
