
all:
	bnfc -m --c ../grammars/kbasic.cf -o chip8
	bnfc -m --java ../grammars/kbasic.cf -o java
