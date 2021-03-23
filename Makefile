all: tic

tic: main.c
	cc main.c -o tic -Wall -Werror -g
