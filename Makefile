all:
	gcc -g -Wall src/main.c -o hanoi -lncurses
clean:
	rm hanoi
