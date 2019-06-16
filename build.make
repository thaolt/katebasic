

all: chip8

chip8: grammars
	cp -f ../chip8/kbc/* ./chip8/
	make -C chip8
	mv chip8/kbc .

java: grammars
	echo "java in not implemeted yet"

cpp: grammars
	make -C cpp

grammars:
	make -f ../grammars/Makefile
