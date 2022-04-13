CXXFLAGS += -Iheaders -O2 -ljsoncpp -pthread -g
# CXXFLAGS += -Wall

HEADERS = ant.h pheromone.h utils.h world.h entity.h semaphore.h tile.h
OBJECTS = ant.o pheromone.o utils.o world.o food.o tile.o

.PHONY: all
all: main

main: .build/main.o $(addprefix .build/,$(OBJECTS))
	$(CXX) .build/main.o $(addprefix .build/,$(OBJECTS)) -o $@ $(CXXFLAGS)

.build/%.o: src/%.cpp $(addprefix headers/,$(HEADERS)) | .build/
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.build/main.o: src/main.cpp $(addprefix headers/,$(HEADERS)) | .build/
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.build/:
	mkdir -p $@

clean:
	rm -rf .build
	rm -f main
