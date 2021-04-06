all: tic

tic: 
	cc src/main.c src/tic.c -o tic -Wall -Werror -g -Iinclude

.PHONY: clean
clean:
	rm tic
