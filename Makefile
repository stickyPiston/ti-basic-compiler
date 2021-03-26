all: tic

tic: src/main.c
	cc src/main.c -o tic -Wall -Werror -g

.PHONY: clean
clean:
	rm tic
