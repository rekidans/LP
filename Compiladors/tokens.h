#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: practica.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define ENDWHILE 2
#define WHILE 3
#define ENDIF 4
#define IF 5
#define LOGPLOT 6
#define PLOT 7
#define NORMALIZE 8
#define POP 9
#define PUSH 10
#define NOT 11
#define AND 12
#define OR 13
#define CHECK 14
#define AMEND 15
#define ITH 16
#define EMPTY 17
#define NUM 18
#define ID 19
#define ASSIG 20
#define DIFF 21
#define EQUAL 22
#define COMA 23
#define CONC 24
#define OCLA 25
#define TCLA 26
#define OPAR 27
#define TPAR 28
#define GREAT 29
#define LESS 30
#define SPACE 31

#ifdef __USE_PROTOS
void plots(AST**_root);
#else
extern void plots();
#endif

#ifdef __USE_PROTOS
void linterpretation(AST**_root);
#else
extern void linterpretation();
#endif

#ifdef __USE_PROTOS
void instruction(AST**_root);
#else
extern void instruction();
#endif

#ifdef __USE_PROTOS
void while_bucle(AST**_root);
#else
extern void while_bucle();
#endif

#ifdef __USE_PROTOS
void if_statement(AST**_root);
#else
extern void if_statement();
#endif

#ifdef __USE_PROTOS
void block(AST**_root);
#else
extern void block();
#endif

#ifdef __USE_PROTOS
void condition(AST**_root);
#else
extern void condition();
#endif

#ifdef __USE_PROTOS
void orcondition(AST**_root);
#else
extern void orcondition();
#endif

#ifdef __USE_PROTOS
void andcondition(AST**_root);
#else
extern void andcondition();
#endif

#ifdef __USE_PROTOS
void opboolean(AST**_root);
#else
extern void opboolean();
#endif

#ifdef __USE_PROTOS
void comparation(AST**_root);
#else
extern void comparation();
#endif

#ifdef __USE_PROTOS
void value(AST**_root);
#else
extern void value();
#endif

#ifdef __USE_PROTOS
void datafunction(AST**_root);
#else
extern void datafunction();
#endif

#ifdef __USE_PROTOS
void boolfunction(AST**_root);
#else
extern void boolfunction();
#endif

#ifdef __USE_PROTOS
void function(AST**_root);
#else
extern void function();
#endif

#ifdef __USE_PROTOS
void pop_amend(AST**_root);
#else
extern void pop_amend();
#endif

#ifdef __USE_PROTOS
void push_normalize(AST**_root);
#else
extern void push_normalize();
#endif

#ifdef __USE_PROTOS
void plot(AST**_root);
#else
extern void plot();
#endif

#ifdef __USE_PROTOS
void data(AST**_root);
#else
extern void data();
#endif

#ifdef __USE_PROTOS
void dataset(AST**_root);
#else
extern void dataset();
#endif

#ifdef __USE_PROTOS
void points(AST**_root);
#else
extern void points();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType setwd2[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType setwd3[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType zzerr12[];
extern SetWordType zzerr13[];
extern SetWordType setwd4[];
extern SetWordType zzerr14[];
extern SetWordType zzerr15[];
extern SetWordType zzerr16[];
extern SetWordType setwd5[];
