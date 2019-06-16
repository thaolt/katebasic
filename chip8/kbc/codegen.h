#ifndef CODEGEN_H
#define CODEGEN_H

#include "Absyn.h"

void visitProgram(Program p);
void visitStmt(Stmt p);
void visitListStmt(ListStmt p);
void visitParam(Param p);
void visitListParam(ListParam p);
void visitDecl(Decl p);
void visitListDecl(ListDecl p);
void visitArg(Arg p);
void visitListArg(ListArg p);
void visitLoop(Loop p);
void visitBranch(Branch p);
void visitExp(Exp p);
void visitConst(Const p);
void visitListConst(ListConst p);
void visitNumber(Number p);
void visitType(Type p);

void visitId(Id p);void visitLlabel(Llabel p);void visitNumbin(Numbin p);void visitNumhex(Numhex p);void visitNumdec(Numdec p);
void visitIdent(Ident i);
void visitInteger(Integer i);
void visitDouble(Double d);
void visitChar(Char c);
void visitString(String s);

#endif

