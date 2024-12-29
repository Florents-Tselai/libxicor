CC = gcc

%.o: %.c
	$(CC) -shared $< -o $@

xicor-example: xicor_example.c xicor.c
	$(CC) $^ -O3 -Wall -I. -lm -L. -o $@

all: xicor.o xicor-example

clean:
	@rm *.o
	@rm xicor-example
