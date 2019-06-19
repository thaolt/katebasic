
all: kas

kas: fasmg.o kas.o
	gcc -m32 fasmg.o kas.o -o kas

fasmg.o:
	fasmg fasmg.asm

kas.o:
	gcc -m32 -c kas.c