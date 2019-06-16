#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include "codegen.h"
#include "uthash.h"
#include "utarray.h"

#define HEADER  0
#define GLOBAL  1
#define START   2
#define FUNC    3
#define MEMSTK  4


struct globals { /* global variables */
    char ident[50];
    char value;
    UT_hash_handle hh; /* makes this structure hashable */
};

struct globals *globals = NULL;


UT_array *oheaders;
UT_array *oglobals;
UT_array *ostart;
UT_array *ofunc;
UT_array *omemstack;

char cnumRet = 0;
char is_global_decl = 0;
char expRetType = 0;
char expRetValue = 0;
char idRetValue[50] = {0};
char is_exp_const = 0;

/* scopes */

void initCodeGen()
{
  char* line = NULL;
  utarray_new(oheaders, &ut_str_icd);
  line = "include 'c8.asm'";    utarray_push_back(oheaders, &line);
  line = "org 0x200";           utarray_push_back(oheaders, &line); 
  line = "";                    utarray_push_back(oheaders, &line);
  line = "jmp start";           utarray_push_back(oheaders, &line);

  utarray_new(oglobals, &ut_str_icd);
  line = "";                    utarray_push_back(oglobals, &line);
  line = "global:";             utarray_push_back(oglobals, &line); 

  utarray_new(ostart,   &ut_str_icd);
  line = "";                    utarray_push_back(ostart, &line);
  line = "start:";              utarray_push_back(ostart, &line); 

  utarray_new(ofunc,    &ut_str_icd);
  line = "";                    utarray_push_back(ofunc, &line);
  line = "userfunc:";               utarray_push_back(ofunc, &line); 
  
  utarray_new(omemstack,&ut_str_icd);
  line = "";                    utarray_push_back(omemstack, &line);
  line = "memstk:";             utarray_push_back(omemstack, &line); 
}


void visitProgram(Program _p_)
{
  char* line = 0;
  switch(_p_->kind)
  {
  case is_Prog:
    visitListStmt(_p_->u.prog_.liststmt_);

    char **p = 0;
    while ( (p=(char**)utarray_next(oheaders,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(oglobals,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(ostart,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(ofunc,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(omemstack,p))) {
      printf("%s\n",*p);
    }

    utarray_free(oheaders);
    utarray_free(oglobals);
    utarray_free(ostart);
    utarray_free(ofunc);
    utarray_free(omemstack);
    break;

  default:
    fprintf(stderr, "Error: bad kind field when printing Program!\n");
    exit(1);
  }
}

void visitStmt(Stmt _p_)
{
  char *line = malloc(100);
  switch(_p_->kind)
  {
    case is_SEval:
    /* Code for SEval Goes Here */
    visitExp(_p_->u.seval_.exp_);
    break;  case is_SAsgArr:
    /* Code for SAsgArr Goes Here */
    visitId(_p_->u.sasgarr_.id_);
    visitNumber(_p_->u.sasgarr_.number_);
    break;  
    case is_SAsgVar:
      visitId(_p_->u.sasgvar_.id_);
    break;
    case is_SAsg:
      visitStmt(_p_->u.sasg_.stmt_);
      visitExp(_p_->u.sasg_.exp_);
      snprintf(line, 100, "\tmov\tI,\tglobal.%s", idRetValue);
      utarray_push_back(ostart, &line);
      snprintf(line, 100, "\tregd\tv0");
      utarray_push_back(ostart, &line);
    break;
    case is_SDecl:
    /* Code for SDecl Goes Here */
    visitDecl(_p_->u.sdecl_.decl_);
    break;
    case is_SBran:
    /* Code for SBran Goes Here */
    visitBranch(_p_->u.sbran_.branch_);
    break;  case is_SLoop:
    /* Code for SLoop Goes Here */
    visitLoop(_p_->u.sloop_.loop_);
    break;  case is_SGoto:
    /* Code for SGoto Goes Here */
    visitId(_p_->u.sgoto_.id_);
    break;  case is_SGoSb:
    /* Code for SGoSb Goes Here */
    visitId(_p_->u.sgosb_.id_);
    break;  case is_SAsm:
    /* Code for SAsm Goes Here */
    visitString(_p_->u.sasm_.string_);
    break;  case is_SCall:
    /* Code for SCall Goes Here */
    visitId(_p_->u.scall_.id_);
    visitListParam(_p_->u.scall_.listparam_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Stmt!\n");
    exit(1);
  }
  free(line);
}

void visitListStmt(ListStmt liststmt)
{
  while(liststmt != 0)
  {
    /* Code For ListStmt Goes Here */
    visitStmt(liststmt->stmt_);
    liststmt = liststmt->liststmt_;
  }
}

void visitParam(Param _p_)
{
  switch(_p_->kind)
  {
  case is_FPar:
    /* Code for FPar Goes Here */
    visitExp(_p_->u.fpar_.exp_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Param!\n");
    exit(1);
  }
}

void visitListParam(ListParam listparam)
{
  while(listparam != 0)
  {
    /* Code For ListParam Goes Here */
    visitParam(listparam->param_);
    listparam = listparam->listparam_;
  }
}




void visitDecl(Decl _p_)
{
  char *line = malloc(100);

  switch(_p_->kind)
  {
    case is_DLetA:
      visitId(_p_->u.dleta_.id_);
      if (is_global_decl) {
        
        memset(line, 0, 100);
        strcat(line, "\t.");
        strcat(line, idRetValue);

        visitExp(_p_->u.dleta_.exp_);
        strcat(line, "\tdb\t");

        if (is_exp_const) {
          char num[50] = {0};
          snprintf(num, 50,"%d",cnumRet);
          strcat(line, num);
        } else
          strcat(line, "0");

        utarray_push_back(oglobals, &line); 
      } else {
        visitExp(_p_->u.dleta_.exp_);
      }
    break;  
    case is_DLetTA:
    /* Code for DLetTA Goes Here */
    visitId(_p_->u.dletta_.id_);
    visitType(_p_->u.dletta_.type_);
    visitExp(_p_->u.dletta_.exp_);
    break;  case is_DLetT:
    /* Code for DLetT Goes Here */
    visitId(_p_->u.dlett_.id_);
    visitType(_p_->u.dlett_.type_);
    break;  
    case is_DLetB:
      visitId(_p_->u.dletb_.id_);
      if (is_global_decl) {
        memset(line, 0, 100);
        strcat(line, "\t.");
        strcat(line, idRetValue);
        strcat(line, "\tdb\t0");
        utarray_push_back(oglobals, &line); 
      } else {
      }
    break;

    case is_DGlbl:
    is_global_decl = 1;
    visitDecl(_p_->u.dglbl_.decl_);
    is_global_decl = 0;
    break;  
    case is_DLet:
    /* Code for DLet Goes Here */
    visitDecl(_p_->u.dlet_.decl_);
    break;
    case is_DSub:
    /* Code for DSub Goes Here */
    visitId(_p_->u.dsub_.id_);
    visitListStmt(_p_->u.dsub_.liststmt_);
    break;  case is_DFnRt:
    /* Code for DFnRt Goes Here */
    visitExp(_p_->u.dfnrt_.exp_);
    break;  case is_DFnNR:
    /* Code for DFnNR Goes Here */
    break;  case is_DFunc:
    /* Code for DFunc Goes Here */
    visitId(_p_->u.dfunc_.id_);
    visitListArg(_p_->u.dfunc_.listarg_);
    visitListStmt(_p_->u.dfunc_.liststmt_);
    visitDecl(_p_->u.dfunc_.decl_);
    break;  case is_DStruct:
    /* Code for DStruct Goes Here */
    visitId(_p_->u.dstruct_.id_);
    visitListDecl(_p_->u.dstruct_.listdecl_);
    break;  case is_DLbl:
    /* Code for DLbl Goes Here */
    visitLlabel(_p_->u.dlbl_.llabel_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Decl!\n");
    exit(1);
  }
}

void visitListDecl(ListDecl listdecl)
{
  while(listdecl != 0)
  {
    /* Code For ListDecl Goes Here */
    visitDecl(listdecl->decl_);
    listdecl = listdecl->listdecl_;
  }
}

void visitArg(Arg _p_)
{
  switch(_p_->kind)
  {
  case is_FArg:
    /* Code for FArg Goes Here */
    visitId(_p_->u.farg_.id_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Arg!\n");
    exit(1);
  }
}

void visitListArg(ListArg listarg)
{
  while(listarg != 0)
  {
    /* Code For ListArg Goes Here */
    visitArg(listarg->arg_);
    listarg = listarg->listarg_;
  }
}

void visitLoop(Loop _p_)
{
  switch(_p_->kind)
  {
  case is_LForN:
    /* Code for LForN Goes Here */
    visitExp(_p_->u.lforn_.exp_);
    visitListStmt(_p_->u.lforn_.liststmt_);
    break;  case is_LForO:
    /* Code for LForO Goes Here */
    visitListStmt(_p_->u.lforo_.liststmt_);
    break;  case is_LFor:
    /* Code for LFor Goes Here */
    visitDecl(_p_->u.lfor_.decl_);
    visitExp(_p_->u.lfor_.exp_);
    visitLoop(_p_->u.lfor_.loop_);
    break;  case is_LWhile:
    /* Code for LWhile Goes Here */
    visitExp(_p_->u.lwhile_.exp_);
    visitListStmt(_p_->u.lwhile_.liststmt_);
    break;  case is_LRepeat:
    /* Code for LRepeat Goes Here */
    visitListStmt(_p_->u.lrepeat_.liststmt_);
    visitExp(_p_->u.lrepeat_.exp_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Loop!\n");
    exit(1);
  }
}

void visitBranch(Branch _p_)
{
  switch(_p_->kind)
  {
  case is_BIfEn:
    /* Code for BIfEn Goes Here */
    break;  case is_BIfEl:
    /* Code for BIfEl Goes Here */
    visitListStmt(_p_->u.bifel_.liststmt_);
    visitBranch(_p_->u.bifel_.branch_);
    break;  case is_BIfEI:
    /* Code for BIfEI Goes Here */
    visitExp(_p_->u.bifei_.exp_);
    visitListStmt(_p_->u.bifei_.liststmt_);
    visitBranch(_p_->u.bifei_.branch_);
    break;  case is_BIf:
    /* Code for BIf Goes Here */
    visitExp(_p_->u.bif_.exp_);
    visitListStmt(_p_->u.bif_.liststmt_);
    visitBranch(_p_->u.bif_.branch_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Branch!\n");
    exit(1);
  }
}

void visitExp(Exp _p_)
{
  char *line = malloc(100);
  memset(line, 0, 100);
  switch(_p_->kind)
  {
  case is_ELgOr:
    /* Code for ELgOr Goes Here */
    visitExp(_p_->u.elgor_.exp_1);
    visitExp(_p_->u.elgor_.exp_2);
    break;  case is_ELgAnd:
    /* Code for ELgAnd Goes Here */
    visitExp(_p_->u.elgand_.exp_1);
    visitExp(_p_->u.elgand_.exp_2);
    break;  case is_EBitOr:
    /* Code for EBitOr Goes Here */
    visitExp(_p_->u.ebitor_.exp_1);
    visitExp(_p_->u.ebitor_.exp_2);
    break;  case is_EBitXor:
    /* Code for EBitXor Goes Here */
    visitExp(_p_->u.ebitxor_.exp_1);
    visitExp(_p_->u.ebitxor_.exp_2);
    break;  case is_EBitAnd:
    /* Code for EBitAnd Goes Here */
    visitExp(_p_->u.ebitand_.exp_1);
    visitExp(_p_->u.ebitand_.exp_2);
    break;  case is_ELgEq:
    /* Code for ELgEq Goes Here */
    visitExp(_p_->u.elgeq_.exp_1);
    visitExp(_p_->u.elgeq_.exp_2);
    break;  case is_ELgNe:
    /* Code for ELgNe Goes Here */
    visitExp(_p_->u.elgne_.exp_1);
    visitExp(_p_->u.elgne_.exp_2);
    break;  case is_ELgLt:
    /* Code for ELgLt Goes Here */
    visitExp(_p_->u.elglt_.exp_1);
    visitExp(_p_->u.elglt_.exp_2);
    break;  case is_ELgGt:
    /* Code for ELgGt Goes Here */
    visitExp(_p_->u.elggt_.exp_1);
    visitExp(_p_->u.elggt_.exp_2);
    break;  case is_ELgLte:
    /* Code for ELgLte Goes Here */
    visitExp(_p_->u.elglte_.exp_1);
    visitExp(_p_->u.elglte_.exp_2);
    break;  case is_ELgGte:
    /* Code for ELgGte Goes Here */
    visitExp(_p_->u.elggte_.exp_1);
    visitExp(_p_->u.elggte_.exp_2);
    break;  case is_EBitShl:
    /* Code for EBitShl Goes Here */
    visitExp(_p_->u.ebitshl_.exp_1);
    visitExp(_p_->u.ebitshl_.exp_2);
    break;  case is_EBitShr:
    /* Code for EBitShr Goes Here */
    visitExp(_p_->u.ebitshr_.exp_1);
    visitExp(_p_->u.ebitshr_.exp_2);
    break;  
    case is_EAdd: {
      /* Code for EAdd Goes Here */
      visitExp(_p_->u.eadd_.exp_1);
      char exp1_const = is_exp_const;
      char cnum1 = cnumRet;
      visitExp(_p_->u.eadd_.exp_2);
      char exp2_const = is_exp_const;
      char cnum2 = cnumRet;
      if (exp1_const && exp2_const) {
        cnumRet = cnum1 + cnum2;
        if (!is_global_decl) {
          snprintf(line, 100,"\tmov\tv0,\t%d",cnumRet);
          utarray_push_back(ostart, &line);
        } else {
          snprintf(line, 100,"\tmov\tv0,\t%d",cnumRet);
          utarray_push_back(ostart, &line);
        }
      } else {
        is_exp_const = 0;
      }
    } break;  
    case is_ESub:
    /* Code for ESub Goes Here */
    visitExp(_p_->u.esub_.exp_1);
    visitExp(_p_->u.esub_.exp_2);
    break;  case is_EDiv:
    /* Code for EDiv Goes Here */
    visitExp(_p_->u.ediv_.exp_1);
    visitExp(_p_->u.ediv_.exp_2);
    break;  case is_EMul:
    /* Code for EMul Goes Here */
    visitExp(_p_->u.emul_.exp_1);
    visitExp(_p_->u.emul_.exp_2);
    break;  case is_EMod:
    /* Code for EMod Goes Here */
    visitExp(_p_->u.emod_.exp_1);
    visitExp(_p_->u.emod_.exp_2);
    break;  case is_EConst:
    /* Code for EConst Goes Here */
    visitConst(_p_->u.econst_.const_);
    is_exp_const = 1;
    break;  case is_EVar:
    /* Code for EVar Goes Here */
    visitId(_p_->u.evar_.id_);
    is_exp_const = 0;
    break;  case is_EArrV:
    /* Code for EArrV Goes Here */
    visitId(_p_->u.earrv_.id_);
    visitNumber(_p_->u.earrv_.number_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Exp!\n");
    exit(1);
  }
}

void visitConst(Const _p_)
{
  switch(_p_->kind)
  {
  case is_CNum:
    /* Code for CNum Goes Here */
    visitNumber(_p_->u.cnum_.number_);
    break;  case is_CChr:
    /* Code for CChr Goes Here */
    visitChar(_p_->u.cchr_.char_);
    break;  case is_CArr:
    /* Code for CArr Goes Here */
    visitListConst(_p_->u.carr_.listconst_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Const!\n");
    exit(1);
  }
}

void visitListConst(ListConst listconst)
{
  while(listconst != 0)
  {
    /* Code For ListConst Goes Here */
    visitConst(listconst->const_);
    listconst = listconst->listconst_;
  }
}

void visitNumber(Number _p_)
{
  switch(_p_->kind)
  {
  case is_NDec:
    /* Code for NDec Goes Here */
    visitNumdec(_p_->u.ndec_.numdec_);
    break;  case is_NBin:
    /* Code for NBin Goes Here */
    visitNumbin(_p_->u.nbin_.numbin_);
    break;  case is_NHex:
    /* Code for NHex Goes Here */
    visitNumhex(_p_->u.nhex_.numhex_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Number!\n");
    exit(1);
  }
}

void visitType(Type _p_)
{
  switch(_p_->kind)
  {
  case is_TByte:
    /* Code for TByte Goes Here */
    break;  case is_TWord:
    /* Code for TWord Goes Here */
    break;  case is_TDWord:
    /* Code for TDWord Goes Here */
    break;  case is_TQWord:
    /* Code for TQWord Goes Here */
    break;  case is_TDQWord:
    /* Code for TDQWord Goes Here */
    break;  case is_TArr:
    /* Code for TArr Goes Here */
    visitType(_p_->u.tarr_.type_);
    visitNumber(_p_->u.tarr_.number_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Type!\n");
    exit(1);
  }
}



void visitId(Id p)
{
  strncpy(idRetValue, p, 50);
}
void visitLlabel(Llabel p)
{
  /* Code for Llabel Goes Here */
}
void visitNumbin(Numbin p)
{
  /* Code for Numbin Goes Here */
}
void visitNumhex(Numhex p)
{
  /* Code for Numhex Goes Here */
}
void visitNumdec(Numdec p)
{
  cnumRet = atoi(p);
}
void visitIdent(Ident i)
{
  /* Code for Ident Goes Here */
}
void visitInteger(Integer i)
{
  /* Code for Integer Goes Here */
}
void visitDouble(Double d)
{
  /* Code for Double Goes Here */
}
void visitChar(Char c)
{
  /* Code for Char Goes Here */
}
void visitString(String s)
{
  /* Code for String Goes Here */
}

