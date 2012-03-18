default: Prog

ClientMod.o: ClientMod.cpp ClientMod.hpp
	g++ -g -Wall -c ClientMod.cpp -o ClientMod.o

BotMod.o: BotMod.cpp BotMod.hpp
	g++ -g -Wall -c BotMod.cpp -o BotMod.o

Prog: ClientMod.o BotMod.o
	g++ -g -Wall BotMod.o ClientMod.o -o Prog

clean:
	rm -f *.o Prog *~

