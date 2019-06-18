CC = gcc
CCFLAGS = -g -W -Wall -Iuthash/include

FLEX = flex
FLEX_OPTS = -Pkbasic

BISON = bison
BISON_OPTS = -t -pkbasic

OBJS = Absyn.o Lexer.o Parser.o Printer.o

.PHONY: clean distclean

all: Testkbasic

clean:
	rm -f *.o kbc Testkbasic kbasic.aux kbasic.log kbasic.pdf kbasic.dvi kbasic.ps kbasic

distclean: clean
	rm -f kbc.c codegen.c codegen.h Absyn.h Absyn.c Test.c Parser.c Parser.h Lexer.c Skeleton.c Skeleton.h Printer.c Printer.h Makefile kbasic.l kbasic.y kbasic.tex 

Testkbasic: ${OBJS} Test.o
	@echo "Linking Testkbasic..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o Testkbasic

kbc: Testkbasic ${OBJS} kbc.o codegen.o
	@echo "Linking kbc..."
	${CC} ${CCFLAGS} ${OBJS} codegen.o kbc.o -o kbc

Absyn.o: Absyn.c Absyn.h
	${CC} ${CCFLAGS} -c Absyn.c

Lexer.c: kbasic.l
	${FLEX} ${FLEX_OPTS} -oLexer.c kbasic.l

Parser.c: kbasic.y
	${BISON} ${BISON_OPTS} kbasic.y -o Parser.c

Lexer.o: Lexer.c Parser.h
	${CC} ${CCFLAGS} -c Lexer.c 

Parser.o: Parser.c Absyn.h
	${CC} ${CCFLAGS} -c Parser.c

Printer.o: Printer.c Printer.h Absyn.h
	${CC} ${CCFLAGS} -c Printer.c

Test.o: Test.c Parser.h Printer.h Absyn.h
	${CC} ${CCFLAGS} -c Test.c

kbc.o: kbc.c Parser.h Printer.h Absyn.h
	${CC} ${CCFLAGS} -c kbc.c

codegen.o: codegen.c Parser.h Printer.h Absyn.h
	${CC} ${CCFLAGS} -c codegen.c

codegen.c:
	cp -f ../../chip8/kbc/codegen.c .

codegen.h:
	cp -f ../../chip8/kbc/codegen.h .
