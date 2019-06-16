

all: chip8

chip8: grammars
	cp -rf ../chip8/kbc/* ./chip8/
	make -C chip8 Testkbasic
	@echo "=========== Excuting testkbasic parser on \"chip8/tests/simple.bas\""
	./chip8/Testkbasic chip8/tests/simple.bas 
	@echo "==========="
	make -C chip8 kbc
	mv chip8/kbc .
	./kbc chip8/tests/simple.bas

java: grammars
	echo "java in not implemeted yet"

cpp: grammars
	make -C cpp

grammars:
	make -f ../grammars/Makefile
