p0: main_race.o operations_race.o
	g++ main_race.o operations_race.o -lpthread -o p0

p1: main.o operations.o
	g++ main.o operations.o -lpthread -o p1