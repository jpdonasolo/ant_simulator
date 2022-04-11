CXFLAGS = -Iheaders -O2 -ljsoncpp -pthread # -Wall

main: main.o ant.o pheromone.o utils.o world.o
	g++ main.o ant.o pheromone.o utils.o world.o $(CXFLAGS) -o main

main.o: src/main.cpp
	g++ -c src/main.cpp $(CXFLAGS)

utils.o: src/utils.cpp
	g++ -c src/utils.cpp $(CXFLAGS)

world.o: src/world.cpp
	g++ -c src/world.cpp $(CXFLAGS)

ant.o: src/ant.cpp headers/ant.h
	g++ -c src/ant.cpp $(CXFLAGS)

pheromone.o: src/pheromone.cpp headers/pheromone.h
	g++ -c src/pheromone.cpp $(CXFLAGS)

clean:
	rm *.o
