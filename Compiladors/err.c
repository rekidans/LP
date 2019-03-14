/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: practica.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <list>
#include <vector>
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);
#define zzSET_SIZE 8
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[32]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"ENDWHILE",
	/* 03 */	"WHILE",
	/* 04 */	"ENDIF",
	/* 05 */	"IF",
	/* 06 */	"LOGPLOT",
	/* 07 */	"PLOT",
	/* 08 */	"NORMALIZE",
	/* 09 */	"POP",
	/* 10 */	"PUSH",
	/* 11 */	"NOT",
	/* 12 */	"AND",
	/* 13 */	"OR",
	/* 14 */	"CHECK",
	/* 15 */	"AMEND",
	/* 16 */	"ITH",
	/* 17 */	"EMPTY",
	/* 18 */	"NUM",
	/* 19 */	"ID",
	/* 20 */	"ASSIG",
	/* 21 */	"DIFF",
	/* 22 */	"EQUAL",
	/* 23 */	"COMA",
	/* 24 */	"CONC",
	/* 25 */	"OCLA",
	/* 26 */	"TCLA",
	/* 27 */	"OPAR",
	/* 28 */	"TPAR",
	/* 29 */	"GREAT",
	/* 30 */	"LESS",
	/* 31 */	"SPACE"
};
SetWordType zzerr1[8] = {0x0,0x87,0x8,0x2, 0x0,0x0,0x0,0x0};
SetWordType zzerr2[8] = {0xe8,0x0,0x8,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd1[32] = {0x0,0xc5,0xc4,0xc2,0xc4,0xc2,0xe2,
	0xe2,0x10,0x10,0x10,0x0,0x0,0x0,0x0,
	0x10,0x0,0x0,0x0,0xca,0x0,0x0,0x0,
	0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,
	0x0};
SetWordType zzerr3[8] = {0x0,0x48,0xb,0xa, 0x0,0x0,0x0,0x0};
SetWordType zzerr4[8] = {0x0,0x40,0xb,0xa, 0x0,0x0,0x0,0x0};
SetWordType setwd2[32] = {0x0,0x1,0x3,0x1,0x3,0x1,0x1,
	0x1,0x0,0x0,0x0,0x0,0x0,0x20,0x84,
	0x0,0x44,0x84,0x0,0x45,0x0,0x0,0x0,
	0x0,0x0,0x44,0x0,0x4,0x38,0x0,0x0,
	0x0};
SetWordType zzerr5[8] = {0x0,0x0,0x60,0x60, 0x0,0x0,0x0,0x0};
SetWordType zzerr6[8] = {0x0,0x0,0x9,0x2, 0x0,0x0,0x0,0x0};
SetWordType zzerr7[8] = {0x0,0x40,0x2,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr8[8] = {0x0,0x87,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr9[8] = {0x0,0x83,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd3[32] = {0x0,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x40,0x40,0x0,0x0,0x3b,0x3b,0x0,
	0x40,0x0,0x0,0x0,0x84,0x0,0x18,0x18,
	0x80,0x0,0x4,0x0,0x0,0xbb,0x18,0x18,
	0x0};
SetWordType zzerr10[8] = {0x0,0x87,0x8,0x22, 0x0,0x0,0x0,0x0};
SetWordType zzerr11[8] = {0xc0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr12[8] = {0x0,0x87,0x8,0x2, 0x0,0x0,0x0,0x0};
SetWordType zzerr13[8] = {0x0,0x0,0x8,0x2, 0x0,0x0,0x0,0x0};
SetWordType zzerr14[8] = {0x0,0x0,0x8,0x2, 0x0,0x0,0x0,0x0};
SetWordType setwd4[32] = {0x0,0xc9,0xc9,0xc9,0xc9,0xc9,0xc9,
	0xc9,0x24,0x24,0x24,0x0,0x80,0x80,0x0,
	0x24,0x0,0x0,0x0,0xdb,0x0,0x80,0x80,
	0x89,0x0,0x12,0x0,0x0,0x89,0x80,0x80,
	0x0};
SetWordType setwd5[32] = {0x0,0x1,0x1,0x1,0x1,0x1,0x1,
	0x1,0x0,0x0,0x0,0x0,0x1,0x1,0x0,
	0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x1,
	0x3,0x1,0x0,0x2,0x0,0x3,0x1,0x1,
	0x0};