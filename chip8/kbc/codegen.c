#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <stdint.h> 
#include <stdarg.h>
#include <stdbool.h>

#include "codegen.h"

#define ERETCONST 0
#define ERETVAR   1
#define ERETREG   2

#define CRETNUM   0
#define CRETCHAR  0
#define CRETARR   0

#include "uthash.h"
#include "utarray.h"

/* ======= RETURNS */
/* --- Global */
uint8_t isGlobalDeclare   = 0;
/* --- Ids */
char    retId[50]         = {0};
/* --- Numbers */
uint8_t retNum            = 0;
/* --- Constants */
uint8_t retConst          = 0;
/* --- Expressions */
uint8_t retExpType        = 0;
uint8_t retExpConst       = 0;
char    retExpId[50]      = {0};

/* --- Declarations */
/* ======= OUTPUT */
UT_array *oheaders;
UT_array *oglobals;
UT_array *omain;
UT_array *okfunc;
UT_array *ouserfunc;
UT_array *omemstack;
/* ======= LOOKUP TABLES */

/* ======= SCOPES */

struct scope {
    char id[50];                    /* key */
    UT_array *oasm;
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct scope *scopes = NULL;    /* important! initialize to NULL */
struct scope *cscope = NULL;


/* ======= UTILITIES */
static void _asm(UT_array *section, const char *fmt, ...) {
  char *line = (char*) calloc(100, sizeof(char));
  
  va_list args;

  va_start(args, fmt);
  vsnprintf(line, 100, fmt, args);
  va_end(args);

  utarray_push_back(section, &line);

  free(line);
}

/* ======= CHIP8 FUNCTION (KFUNC) */
#include "kfunc.c"

/* ======= INITIALIZATION */
void initCodeGen()
{
  char* line = NULL;
  
  utarray_new(oheaders, &ut_str_icd);
  _asm(oheaders, "include 'c8.asm'");
  _asm(oheaders, "org 0x200");
  _asm(oheaders, "");
  _asm(oheaders, "jmp start");
  _asm(oheaders, "");

  utarray_new(oglobals, &ut_str_icd);
  _asm(oglobals, "");
  _asm(oglobals, "global:");

  struct scope *main = malloc(sizeof(struct scope));
  strcpy(main->id, "main");
  utarray_new(omain,   &ut_str_icd);
  _asm(omain, "");
  _asm(omain, "start:");
  main->oasm = omain;
  HASH_ADD_STR( scopes, id, main );  /* id: name of key field */
  cscope = omain;

  utarray_new(ouserfunc,    &ut_str_icd);
  _asm(ouserfunc, "");
  _asm(ouserfunc, "userfunc:");

  utarray_new(okfunc,    &ut_str_icd);
  _asm(okfunc, "");
  _asm(okfunc, "kfunc:");
  
  utarray_new(omemstack,&ut_str_icd);
  _asm(omemstack, "");
  _asm(omemstack, "memstk:");
}

/* ======= COMMON FUNCTIONALITIES */
void declareGlobal(Decl _p_) {
  char varName[50] = {0};

  /* TODO: check for duplication name */

  isGlobalDeclare = 1;

  Type varType = make_TByte();
  uint8_t valueExpType = ERETCONST;
  uint8_t value = 0;

  switch(_p_->kind)
  {
    case is_DLetA:
      visitId(_p_->u.dleta_.id_);
      strcpy(varName, retId);

      visitExp(_p_->u.dleta_.exp_);
      valueExpType = retExpType;
      if (valueExpType == ERETCONST)
        value = retExpConst;
    break;

    case is_DLetTA:
      visitId(_p_->u.dletta_.id_);
      strcpy(varName, retId);

      visitType(_p_->u.dletta_.type_);

      visitExp(_p_->u.dletta_.exp_);
      valueExpType = retExpType;
      if (valueExpType == ERETCONST)
        value = retExpConst;
    break;

    case is_DLetT:
      visitId(_p_->u.dlett_.id_);
      strcpy(varName, retId);

      visitType(_p_->u.dlett_.type_);
    break;

    case is_DLetB:
      /* Code for DLetB Goes Here */
      visitId(_p_->u.dletb_.id_);
      strcpy(varName, retId);
    break;

    default:
      fprintf(stderr, "Error: bad kind field when printing Decl!\n");
      exit(1);
  }

  _asm(oglobals, "\t.%s\tdb\t%d", varName, value);
  isGlobalDeclare = 0;
}

void loadVariable(Exp e) {
  if (e->kind == is_EVar) {
    _asm(cscope, "\tmov\tI,\tglobal.%s", e->u.evar_.id_);
    _asm(cscope, "\tregl\tv0");
  }
}
/* ======= TREE TRAVERSAL */
void visitProgram(Program _p_)
{
  switch(_p_->kind)
  {
  case is_Prog:
    visitListLine(_p_->u.prog_.listline_);

    _asm(omain, "");
    _asm(omain, "halt:");
    _asm(omain, "\tjmp\thalt");
    char **p = 0;
    while ( (p=(char**)utarray_next(oheaders,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(oglobals,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(omain,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(ouserfunc,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(okfunc,p))) {
      printf("%s\n",*p);
    }

    while ( (p=(char**)utarray_next(omemstack,p))) {
      printf("%s\n",*p);
    }

    utarray_free(oheaders);
    utarray_free(oglobals);
    utarray_free(omain);
    utarray_free(ouserfunc);
    utarray_free(okfunc);
    utarray_free(omemstack);

    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Program!\n");
    exit(1);
  }
}

void visitLine(Line _p_)
{
  switch(_p_->kind)
  {
  case is_PLDecl:
    /* Code for PLDecl Goes Here */
    visitPDecl(_p_->u.pldecl_.pdecl_);
    break;  case is_PStmt:
    /* Code for PStmt Goes Here */
    visitStmt(_p_->u.pstmt_.stmt_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing Line!\n");
    exit(1);
  }
}

void visitListLine(ListLine listline)
{
  while(listline != 0)
  {
    /* Code For ListLine Goes Here */
    visitLine(listline->line_);
    listline = listline->listline_;
  }
}

void visitStmt(Stmt _p_)
{
  switch(_p_->kind)
  {
    case is_SEval:
      /* Code for SEval Goes Here */
      visitExp(_p_->u.seval_.exp_);
    break;
    case is_SAsgArr:
      /* Code for SAsgArr Goes Here */
      visitId(_p_->u.sasgarr_.id_);
      visitExp(_p_->u.sasgarr_.exp_);
    break;
    case is_SAsgVar:
      visitId(_p_->u.sasgvar_.id_);
    break;
    case is_SAsg:
      visitExp(_p_->u.sasg_.exp_);
      visitStmt(_p_->u.sasg_.stmt_);

      if (retExpType == ERETCONST)
        _asm(cscope, "\tmov\tv0,\t%d", retNum);
      else;

      _asm(cscope, "\tmov\tI,\tglobal.%s", retId);
      _asm(cscope, "\tregd\tv0");
    break;
    case is_SDecl:
      /* Code for SDecl Goes Here */
      visitDecl(_p_->u.sdecl_.decl_);
    break;
    case is_SBran:
      /* Code for SBran Goes Here */
      visitBranch(_p_->u.sbran_.branch_);
    break;
    case is_SLoop:
      /* Code for SLoop Goes Here */
      visitLoop(_p_->u.sloop_.loop_);
    break;
    case is_SGoto:
      /* Code for SGoto Goes Here */
      visitId(_p_->u.sgoto_.id_);
    break;
    case is_SGoSb:
      /* Code for SGoSb Goes Here */
      visitId(_p_->u.sgosb_.id_);
    break;
    case is_SAsm:
      visitString(_p_->u.sasm_.string_);
      _asm(cscope, "\t%s", _p_->u.sasm_.string_);
    break;
    case is_SCall:
      /* Code for SCall Goes Here */
      visitId(_p_->u.scall_.id_);
      visitListParam(_p_->u.scall_.listparam_);
    break;
    default:
      fprintf(stderr, "Error: bad kind field when printing Stmt!\n");
      exit(1);
  }
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



void visitPDecl(PDecl _p_)
{
  switch(_p_->kind)
  {
  case is_DFnRt:
      /* Code for DFnRt Goes Here */
    visitExp(_p_->u.dfnrt_.exp_);
    break;  case is_DFnNR:
      /* Code for DFnNR Goes Here */
    break;
    case is_DGlbl:
      declareGlobal(_p_->u.dglbl_.decl_);
    break;
    case is_DSub:
      /* Code for DSub Goes Here */
      visitId(_p_->u.dsub_.id_);
      visitListStmt(_p_->u.dsub_.liststmt_);
    break;
    case is_DFunc:
      /* Code for DFunc Goes Here */
      visitId(_p_->u.dfunc_.id_);
      visitListArg(_p_->u.dfunc_.listarg_);
      visitListStmt(_p_->u.dfunc_.liststmt_);
      visitPDecl(_p_->u.dfunc_.pdecl_);
    break;
    case is_DStruct:
      /* Code for DStruct Goes Here */
      visitId(_p_->u.dstruct_.id_);
      visitListDecl(_p_->u.dstruct_.listdecl_);
    break;
  default:
    fprintf(stderr, "Error: bad kind field when printing PDecl!\n");
    exit(1);
  }
}

void visitListPDecl(ListPDecl listpdecl)
{
  while(listpdecl != 0)
  {
    /* Code For ListPDecl Goes Here */
    visitPDecl(listpdecl->pdecl_);
    listpdecl = listpdecl->listpdecl_;
  }
}

void visitDecl(Decl _p_)
{
  switch(_p_->kind)
  {
  case is_DLetA:
    /* Code for DLetA Goes Here */
    visitId(_p_->u.dleta_.id_);
    visitExp(_p_->u.dleta_.exp_);
    break;  case is_DLetTA:
    /* Code for DLetTA Goes Here */
    visitId(_p_->u.dletta_.id_);
    visitType(_p_->u.dletta_.type_);
    visitExp(_p_->u.dletta_.exp_);
    break;  case is_DLetT:
    /* Code for DLetT Goes Here */
    visitId(_p_->u.dlett_.id_);
    visitType(_p_->u.dlett_.type_);
    break;  case is_DLetB:
    /* Code for DLetB Goes Here */
    visitId(_p_->u.dletb_.id_);
    break;  case is_DLet:
    /* Code for DLet Goes Here */
    visitDecl(_p_->u.dlet_.decl_);
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
  switch(_p_->kind)
  {
    case is_ELgOr:
      /* Code for ELgOr Goes Here */
      visitExp(_p_->u.elgor_.exp_1);
      visitExp(_p_->u.elgor_.exp_2);
    break;
    case is_ELgAnd:
      /* Code for ELgAnd Goes Here */
      visitExp(_p_->u.elgand_.exp_1);
      visitExp(_p_->u.elgand_.exp_2);
    break;
    case is_EBitOr:
      /* Code for EBitOr Goes Here */
      visitExp(_p_->u.ebitor_.exp_1);
      visitExp(_p_->u.ebitor_.exp_2);
    break;
    case is_ELgEq:
      /* Code for ELgEq Goes Here */
      visitExp(_p_->u.elgeq_.exp_1);
      visitExp(_p_->u.elgeq_.exp_2);
    break;
    case is_ELgNe:
      /* Code for ELgNe Goes Here */
      visitExp(_p_->u.elgne_.exp_1);
      visitExp(_p_->u.elgne_.exp_2);
    break;
    case is_ELgLt:
      /* Code for ELgLt Goes Here */
      visitExp(_p_->u.elglt_.exp_1);
      visitExp(_p_->u.elglt_.exp_2);
    break;
    case is_ELgGt:
      /* Code for ELgGt Goes Here */
      visitExp(_p_->u.elggt_.exp_1);
      visitExp(_p_->u.elggt_.exp_2);
    break;
    case is_ELgLte:
      /* Code for ELgLte Goes Here */
      visitExp(_p_->u.elglte_.exp_1);
      visitExp(_p_->u.elglte_.exp_2);
    break;
    case is_ELgGte:
      /* Code for ELgGte Goes Here */
      visitExp(_p_->u.elggte_.exp_1);
      visitExp(_p_->u.elggte_.exp_2);
    break;
    case is_EBitXor:
      /* Code for EBitXor Goes Here */
      visitExp(_p_->u.ebitxor_.exp_1);
      visitExp(_p_->u.ebitxor_.exp_2);
    break;
    case is_EBitAnd:
      /* Code for EBitAnd Goes Here */
      visitExp(_p_->u.ebitand_.exp_1);
      visitExp(_p_->u.ebitand_.exp_2);
    break;
    case is_EBitShl:
      /* Code for EBitShl Goes Here */
      visitExp(_p_->u.ebitshl_.exp_1);
      visitExp(_p_->u.ebitshl_.exp_2);
    break;
    case is_EBitShr:
      /* Code for EBitShr Goes Here */
      visitExp(_p_->u.ebitshr_.exp_1);
      visitExp(_p_->u.ebitshr_.exp_2);
    break;
    case is_EAdd: {
      visitExp(_p_->u.eadd_.exp_1);
      bool exp1Const = retExpType == ERETCONST;
      uint8_t exp1 = retExpConst;

      visitExp(_p_->u.eadd_.exp_2);
      bool exp2Const = retExpType == ERETCONST;
      uint8_t exp2 = retExpConst;

      if (exp1Const && exp2Const) {
        retExpConst = exp1 + exp2;
        if (!isGlobalDeclare)
          _asm(cscope, "\tmov\tv0,\t%d", retExpConst);
      } else {
        if (exp2Const)
          _asm(cscope, "\tmov\tv1,\t%d", exp2);
        else {
          /* load exp2 to v0 */
          loadVariable(_p_->u.eadd_.exp_2);
          _asm(cscope, "\tmov\tv1,\tv0");
        }

        if (exp1Const)
          _asm(cscope, "\tmov\tv0,\t%d", exp1);
        else {
          /* load exp1 to v0 */
          loadVariable(_p_->u.eadd_.exp_1);
        }

        _asm(cscope, "\tadd\tv0,\tv1");
      }
      retExpType = exp1Const && exp2Const ? ERETCONST : ERETREG;
    } break;
    case is_ESub: {
      /* Code for ESub Goes Here */
      visitExp(_p_->u.esub_.exp_1);
      visitExp(_p_->u.esub_.exp_2);
    } break;
    case is_EDiv:
      /* Code for EDiv Goes Here */
      visitExp(_p_->u.ediv_.exp_1);
      visitExp(_p_->u.ediv_.exp_2);
    break;
    case is_EMul: {
      visitExp(_p_->u.emul_.exp_1);
      bool exp1Const = retExpType == ERETCONST;
      uint8_t exp1 = retExpConst;

      visitExp(_p_->u.emul_.exp_2);
      bool exp2Const = retExpType == ERETCONST;
      uint8_t exp2 = retExpConst;

      if (exp1Const && exp2Const) {
        retExpConst = exp1 * exp2;
        if (!isGlobalDeclare)
          _asm(cscope, "\tmov\tv0,\t%d", retExpConst);
      } else {
        if (exp2Const)
          _asm(cscope, "\tmov\tv1,\t%d", exp2);
        else {
          /* load exp2 to v0 */
          loadVariable(_p_->u.eadd_.exp_2);
          _asm(cscope, "\tmov\tv1,\tv0");
        }

        if (exp1Const)
          _asm(cscope, "\tmov\tv0,\t%d", exp1);
        else {
          /* load exp1 to v0 */
          loadVariable(_p_->u.eadd_.exp_1);
        }

        kfuncMultiply();
      }
      retExpType = exp1Const && exp2Const ? ERETCONST : ERETREG;

    } break;
    case is_EMod:
      /* Code for EMod Goes Here */
      visitExp(_p_->u.emod_.exp_1);
      visitExp(_p_->u.emod_.exp_2);
    break;  
    case is_EConst:
      visitConst(_p_->u.econst_.const_);
      retExpType = ERETCONST;
      retExpConst = retNum;
    break;  
    case is_EVar:
      visitId(_p_->u.evar_.id_);
      retExpType = ERETVAR;
    break;
    case is_EArrV:
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
  strcpy(retId, p);
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
  retNum = atoi(p);
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

