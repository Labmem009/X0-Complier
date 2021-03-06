#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN_FILENAME        200        /* max length of file name */
#define MAX_LEN_IDENT           20         /* max length of identifier */
#define MAX_LEN_NUMBER          9          /* max digits of number */
#define MAX_NUM_LINE            100        /* max number of a line's character in input file */
#define MAX_SIZE_TABLE          100        /* max size of symbol table */
#define MAX_NUM_CODE            1000       /* max number of intermidiate code */
#define MAX_SIZE_STACK          10000      /* max size of data stack */
#define MAX_NUM_BREAK           1000       /* max number of 'break' statement */
#define MAX_NUM_CONTINUE        1000       /* max number of 'continue' statement */
#define MAX_NUM_BACKUP          100        /* max number of backups */
#define MAX_DIMENSION           10         /* max dimension of an array */
#define MAX_NUM_FUNCTION        100        /* max number of functions */
#define MAX_LEN_RESERVE_WORD    15         /* max length of reserved word */
#define MAX_LEN_FCTCODE         5          /* max length of function code */
#define FCTCODE_NUM             11         /* number of function code */
#define TERMINAL_SYM_NUM        58         /* number of terminal symbol */
#define RESERVE_WORD_NUM        27         /* number of reserved word */

/*
 * unrecognizable symbol, identifier, main, char, int, if
 * else, while, read, write, +, -
 * *, /, (, ), [, ]
 * {, }, ;, =, >, <
 * >=, <=, ==, !=, integer, double
 * &&, ||, for, %, odd, xor
 * ++, --, !, break, continue, exit
 * switch, case, :, default, do, repeat
 * until, bool, true, false, floating number, const
 * ,, void, return
 */
/* terminal symbol */
typedef enum symbol
{
	nul, ident, mainsym, charsym, intsym, ifsym,
	elsesym, whilesym, readsym, writesym, plus, minus,
	times, slash, lparen, rparen, lbracket, rbracket, 
	lbrace, rbrace, semic, eql, great, less,
	greateql, lesseql, eqleql, neql, intnum, dblsym,
	andsym, orsym, forsym, modsym, oddsym, xorsym,
	incsym, decsym, notsym, brksym, ctnsym, exitsym,
	swtcsym, cassym, colonsym, defausym, dosym, reptsym,
	untlsym, bolsym, truesym, falsesym, doublenum, cstsym,
	comma, voidsym, retsym
}Symbol;

/* function code */
typedef enum fctCode
{
	lit, opr, lod, sto, cal, ini, jmp, jpc, add, sub, tad
}FctCode;

/* intermediate code */
typedef struct instruction
{
	FctCode fct;
	int opr1;
	double opr2;
}Instruction;

/* type of function's return value */
typedef enum retType
{
	retVoid, retInt, retDouble, retChar, retBool
}RetType;

/* data type */
typedef enum dataType
{
	Nul, Int, Double, Char, Bool
}DataType;

/* kind of the object stored in the symbol table */
typedef enum objectKind
{
	intVar, constIntVar, intArray,
	doubleVar, constDoubleVar, doubleArray,
	charVar, constCharVar, charArray,
	boolVar, constBoolVar, boolArray
}ObjectKind;

/* object stored in the symbol table */
typedef struct tableObject
{
	char name[MAX_LEN_IDENT + 1];	/* name of the identifier */
	ObjectKind kind;				/* kind of the identifier */
	int offset;						/* offset relative to current activity record */
	int size[MAX_DIMENSION];	    /* size of each dimension */
	int d;							/* the dimension of array */
	double value;					/* only used by constant */
}TableObject;

/* information of function */
typedef struct functionInfo
{
	char name[MAX_LEN_IDENT + 1];
	TableObject symTable[MAX_SIZE_TABLE];  /* symbol table of the function */
	int tableSize;                         /* size of the symbol table */
	int paraNum;		                   /* number of parameters */
	int startINTCode;		               /* position where the function starts in intermediate code */
	RetType retType;                       /* type of return value */
}FunctionInfo;

/* backup of lexical analysis's status */
typedef struct backUp
{
	FILE* backupFin;                     /* backup the fin */
	Symbol backupSym;				     /* backup the sym */
	char backupCh;                       /* backup the ch */
	int backupPosCh;                     /* backup the posCh */
	char backupLineCache[MAX_NUM_LINE];  /* backup the lineCache */
	int backupChNum;                     /* backup the chNum */
	double backupDoubleNum;              /* backup the doubleNum */
	int backupIntNum;                    /* backup the intNum */
	char backupId[MAX_LEN_IDENT + 1];    /* backup the id */
	int backupIterCode;                  /* backup the codeNum */
}BackUp;

extern char fileName[MAX_LEN_FILENAME + 1];     /* file name */
extern FILE* fin;                               /* file pointer of input file */

/* 
 * variables used in lexical analysis
 */
extern BackUp backups[MAX_NUM_BACKUP];           /* backups of lexical analysis's status */
extern int backUpNum;                            /* number of backups */
extern Symbol sym;                               /* current terminal symbol read by ReadSymbol() */
extern char ch;                                  /* current character read by ReadCh() */
extern int posCh;                                /* position of the character to be read in line cache */
extern char lineCache[MAX_NUM_LINE];             /* line cache using by ReadCh() */
extern int chNum;                                /* number of character in line cache */
extern double doubleNum;                         /* current floating number */
extern int intNum;                               /* current integer */
extern char id[MAX_LEN_IDENT + 1];               /* name of current identifier */
extern Symbol wsym[RESERVE_WORD_NUM];            /* symbol-value corresponding to reserved word */
extern char reserveWord[RESERVE_WORD_NUM][MAX_LEN_RESERVE_WORD];  /* reserved word */

/* 
 * variables used in syntactical analysis
 */
extern int continueList[MAX_NUM_CONTINUE];					/* 'continue' statement to be backfilled */
extern int ctnNum;									        /* number of 'continue' statement to be backfilled */
extern int breakList[MAX_NUM_BREAK];						/* 'break' statement to be backfilled */
extern int brkNum;									        /* number of 'break' statement to be backfilled */
extern Instruction code[MAX_NUM_CODE];						/* intermediate code */
extern int codeNum;                                         /* number of intermediate code */
extern FunctionInfo fctInfo[MAX_NUM_FUNCTION];              /* information of function */
extern int fctNum;                                          /* number of function */
extern char fctCodeString[FCTCODE_NUM][MAX_LEN_FCTCODE];    /* strings corresponding to function code */

/* 
 * lexical analyzer
 */
void ReadSymbol ();
void backup ();     /* backup the status of lexical analysis */
void recover ();    /* recover the status of lexical analysis from backups */

/* 
 * syntactical analyzer
 */
void FunctionCall ();
void FunctionList ();
void VarDeclarationList (int* ptr_offset);
void VarDeclarationStat (int* ptr_offset);
void ConstDeclarationList (int* ptr_offset);
void ConstDeclarationStat (int* ptr_offset);
void StatementList ();
void statement ();
DataType type ();
void IfStat ();
void WhileStat ();
void ReadStat ();
void WriteStat ();
void CompoundStat ();
void ExpressionStat ();
void variable (ObjectKind* ptr_kind, int* ptr_offset, int* ptr_IncOrDec);
void SimpleVariable (ObjectKind* ptr_kind, int* ptr_offset);
void expression ();
void SimpleValue ();
void term ();
void factor ();
void AdditiveExpr ();
void ForStat ();
void ValueExpr ();
void BreakStat ();
void ExitStat ();
void ContinueStat ();
void SwitchStat ();
void DoWhileStat ();
void RepeatStat ();
void ReturnStat ();

/* add a new entry to symbol table */
void EnterTable (ObjectKind k, int offset, int* size, int d, double value);

/* search the position of an identifier in symbol table according to its name */
int FindPosition (char* identName, int pos);

/* search the position of an identifier in symbol table according to its offset */
int FindPosition (int offset, int pos);

/* search the position of a function in fctInfo according to its name */
int FindPosition (char* fctName);

/* error handler */
void ErrorHandler (int error_code);

/* generate an intermediate code */
void GenINTCode (FctCode fct, int opr1, double opr2);	

/* interpret and execute intermidiate code */
void Interpret();