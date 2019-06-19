all: chip8

chip8: kas
	bnfc -m --c ../grammars/kbasic.cf -o chip8

kas:
	bnfc -m --c ../grammars/kasm.cf -o kasm
	make -C kasm
	./kasm/Testkasm && false
