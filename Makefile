CXX = g++
CXFLAGS =  -Iheaders -O2 -ljsoncpp -pthread -g # -Wall

main: main.o ant.o pheromone.o food.o utils.o world.o
	$(CXX) main.o ant.o pheromone.o food.o utils.o world.o $(CXFLAGS) -o main

main.o: src/main.cpp
	$(CXX) -c src/main.cpp $(CXFLAGS)

utils.o: src/utils.cpp
	$(CXX) -c src/utils.cpp $(CXFLAGS)

world.o: src/world.cpp
	$(CXX) -c src/world.cpp $(CXFLAGS)

food.o: src/food.cpp headers/food.h
	g++ -c src/food.cpp $(CXFLAGS)

ant.o: src/ant.cpp headers/ant.h
	$(CXX) -c src/ant.cpp $(CXFLAGS)

pheromone.o: src/pheromone.cpp headers/pheromone.h
	$(CXX) -c src/pheromone.cpp $(CXFLAGS)

clean:
	rm *.o
