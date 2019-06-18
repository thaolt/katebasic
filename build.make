
all: chip8

chip8: grammars
	cp -rf ../chip8/kbc/* ./chip8/
	cp -f ../chip8/kas/c8.asm .
	make -C chip8 Testkbasic
	@echo "=========== Excuting testkbasic parser on \"chip8/tests/simple.bas\""
	./chip8/Testkbasic chip8/tests/simple.bas 
	@echo "==========="
	make -C chip8 kbc
	mv chip8/kbc .
	./kbc chip8/tests/simple.bas | tee simple.asm
	fasmg simple.asm simple.ch8
	chip8run -F ./simple.ch8

java: grammars
	echo "Java in not implemeted yet"

cpp: grammars
	make -C cpp

grammars:
	make -f ../grammars/Makefile
