
all:
	bnfc -m --c ../grammars/kbasic.cf -o chip8
	bnfc -m --java ../grammars/kbasic.cf -o java
	bnfc -m --cpp ../grammars/kbasic.cf -o cpp
