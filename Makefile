main: main.o utils.o world.o
	g++ main.o utils.o world.o -O2 -ljsoncpp -Iheaders -o main

main.o: src/main.cpp
	g++ -c src/main.cpp -O2 -Iheaders

utils.o: src/utils.cpp
	g++ -c src/utils.cpp -O2 -Iheaders

world.o: src/world.cpp
	g++ -c src/world.cpp -O2 -Iheaders

clean:
	rm *.o