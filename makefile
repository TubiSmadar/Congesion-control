# Makefile for TCP project

all: sender measure

measure: measure.c
	gcc -o measure measure.c

sender: sender.c
	gcc -o sender sender.c

clean:
	rm -f *.o sender measure

runs:
	./measure

runc:
	./sender

runs-strace:
	strace -f ./measure

runc-strace:
	strace -f ./sender
