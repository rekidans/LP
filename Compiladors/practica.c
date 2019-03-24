/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt practica.g
 *
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
#define GENAST

#include "ast.h"

#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>


map<string, list<pair<int,int> > > dps;



// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  if(type == ID){
    attr->kind = "ID";
    attr->text = text;
    
  } else {
    attr->kind = text;
    attr->text = "";
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind;
  as->text = attr->text;
  as->right = NULL;
  as->down = NULL;
  return as;
}

AST* createASTstring(AST* sibling , char* etiqueta){
  AST* as = new AST;
  as->kind = etiqueta;
  as->text = "";
  as->right = NULL;
  as->down = sibling;
  return as;
  
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a,int n) {
  AST *c=a->down;
  for (int i=0; c!=NULL && i<n; i++) c=c->right;
  return c;
}

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s)
{
  if (a==NULL) return;
  
  if(a->kind == "ID") cout << a->text;
  else {
    cout<<a->kind;
    if (a->text!="") cout<<"("<<a->text<<")";
  }
  cout<<endl;
  
  AST *i = a->down;
  while (i!=NULL && i->right!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"  |"+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
  
  if (i!=NULL) {
    cout<<s+"  \\__";
    ASTPrintIndent(i,s+"   "+string(i->kind.size()+i->text.size(),' '));
    i=i->right;
  }
}

/// print AST
void ASTPrint(AST *a)
{
  while (a!=NULL) {
    cout<<" ";
    ASTPrintIndent(a,"");
    a=a->right;
  }
}


/// Checks if a dataset is well formed
bool check_plot(list<pair<int,int> >& dp){
  list<pair<int,int> >::iterator it = dp.begin(), it2 = ++dp.begin();
  while(it2 != dp.end()){
    if((*it).first == (*it2).first) return false;
    ++it;
    ++it2;
  }
  return true;
}

/// Normalizes the dataset
void normalize_plot(list<pair<int,int> >& dp){
  list<pair<int,int> >::iterator it = dp.begin();
  int minX = (*it).first, minY = (*it).second;
  ++it;
  for(; it!=dp.end(); ++it){
    if((*it).first < minX) minX = (*it).first;
    if((*it).second < minY) minY = (*it).second;
  }
  for(it = dp.begin(); it != dp.end(); ++it){
    (*it).first-=minX;
    (*it).second-=minY;
  }
}

/// Amends the dataset if needed
void amend_plot(list<pair<int,int> >& dp){
  list<pair<int,int> >::iterator it = dp.begin(), it2 = ++dp.begin();
  while(it2 != dp.end()){
    if((*it).first == (*it2).first) it2 = dp.erase(it2);
    else{
      ++it;
      ++it2;
    }
  }
}

/// Returns the ith element in the dataset.
pair<int,int> get_ith(list<pair<int,int> >& dp, int pos){
  list<pair<int,int> >::iterator it = dp.begin();
  advance(it, pos);
  return *it;
}

/// Prints the given dataset as a list of pairs and as a grid
void draw_plot(const list<pair<int,int> >& dp){
  list<pair<int,int> >::const_iterator it = dp.begin();
  int max;
  if((*it).first > (*it).second) max = (*it).first;
  else max = (*it).second;
  for(; it != dp.end(); ++it){
    if((*it).first > max) max= (*it).first;
    if((*it).second > max) max = (*it).second;
  }
  max+=2;
  vector<vector<string> > plot(max, vector<string>(max));
  //Print de list of points in the dataset
  cout << "[";
  it = dp.begin();
  if(it != dp.end()){
    plot[(*it).first][(*it).second] = "X";
    cout << "<" << (*it).first << "," << (*it).second << ">";
    ++it;
  }
  for(; it != dp.end(); it++){
    plot[(*it).first][(*it).second] = "X";
    cout << " , " << "<" << (*it).first << "," << (*it).second << ">";
  }
  cout << "]" << endl;
  //Draw Plot as a grid
  for(int i = 0; i < max; ++i){
    for(int j = 0; j < max; ++j){
      if(i == 0 and plot[i][j] != "X") plot[i][j] = "|";
      else if(j == 0 and plot[i][j] != "X") plot[i][j] = "-";
      else if (plot[i][j] != "X") plot[i][j] = "·";
    }
  }
  for(int i = max-1; i>= 0; --i){
    for(int j = 0; j < max; ++j){
      cout << plot[j][i];
    }
    cout << endl;
  }
  cout << endl;
}


/// Executes the instructions that return a list of pairs
list<pair<int,int> > list_evaluation (AST *a){
  if(a != NULL){
    if(a->kind == "def"){
      list<pair<int,int> > temp;
      for (int i = 0; child(a, i) != NULL; ++i) {
        list<pair<int,int> > temp2 = list_evaluation(child(a, i));
        temp.insert(temp.end(), temp2.begin(), temp2.end());
      }
      temp.sort();
      return temp;
    }else if(a->kind == "ID"){
      return dps[a->text];
    }else if(a->kind == "literal"){
      list<pair<int,int> > temp;
      for (int i = 0; child(a, i) != NULL; ++i) {
        int f = atoi(child(child(a,i), 0)->kind.c_str());
        int s = atoi(child(child(a,i), 1)->kind.c_str());
        pair<int,int> temp2 = make_pair(f,s);
        temp.push_back(temp2);
      }
      return temp;
    }else if(a->kind == "NORMALIZE"){
      list<pair<int,int> > temp = list_evaluation(child(a,0));
      normalize_plot(temp);
      return temp;
      
            }else if(a->kind == "POP"){
      list<pair<int,int> > temp = list_evaluation(child(a,0));
      if(not temp.empty()) temp.pop_back();
      return temp;
      
            }else if(a->kind == "PUSH"){
      list<pair<int,int> > temp = list_evaluation(child(a,0));
      int f = atoi(child(child(a,1), 0)->kind.c_str());
      int s = atoi(child(child(a,1), 1)->kind.c_str());
      pair<int,int> temp2 = make_pair(f,s);
      temp.push_back(temp2);
      temp.sort();
      return temp;
      
            }else if(a->kind == "AMEND"){
      list<pair<int,int> > temp = list_evaluation(child(a,0));
      amend_plot(temp);
      return temp;
      
            }
    
    }
  
}


/// Execute the boolean and conditional instructions
bool boolean_evaluation(AST *a){
  if(a != NULL) {
    if(a->kind == "NOT"){
      return not boolean_evaluation(child(a,0));
    }else if(a->kind == "OR") {
      return boolean_evaluation(child(a,0)) or boolean_evaluation(child(a,1));
    }else if(a->kind == "AND") {
      return boolean_evaluation(child(a,0)) and boolean_evaluation(child(a,1));
    }else if(a->kind == "CHECK"){
      list<pair<int,int> > temp = list_evaluation(child(a,0));
      return check_plot(temp);
    }else if(a->kind == "EMPTY"){
      return list_evaluation(child(a,0)).empty();
    }else{
      int pos1 = atoi(child(child(a,0), 0)->kind.c_str()), pos2 = atoi(child(child(a,1), 0)->kind.c_str());
      list<pair<int,int> > temp1 = list_evaluation(child(child(a,0), 1)),  temp2 = list_evaluation(child(child(a,1), 1));
      pair<int, int> comp1, comp2;
      if (not temp1.empty() and not temp2.empty()) comp1 = get_ith(temp1, pos1), comp2 = get_ith(temp2, pos2);
      if(a->kind == "=="){
        if(temp1.empty() and temp2.empty()) return true;
        else if (temp1.empty() or temp2.empty()) return false;
        else return comp1.first == comp2.first and comp1.second == comp2.second;
      }else if(a->kind == "!="){
        if(temp1.empty() and temp2.empty()) return false;
        else if (temp1.empty() or temp2.empty()) return true;
        else return comp1.first != comp2.first or comp1.second != comp2.second;
      }else if(a->kind == ">"){
        if(temp1.empty() and temp2.empty()) return false;
        else if (temp1.empty()) return false;
        else if (temp2.empty()) return true;
        else if(comp1.first == comp2.first) return comp1.second > comp2.second;
        else return comp1.first > comp2.first;
      }else if(a->kind == "<"){
        if(temp1.empty() and temp2.empty()) return false;
        else if (temp2.empty()) return false;
        else if (temp1.empty()) return true;
        else if(comp1.first == comp2.first) return comp1.second < comp2.second;
        else return comp1.first < comp2.first;
        
            }
    }
  }
}

/// Executes the general instructions that don't return any value following the AST
void execute(AST *a) {
if (a == NULL) return;
else if (a->kind == "DataPlotsProgram"){
execute(child(a,0));
}else if (a->kind == "IF"){
if(boolean_evaluation(child(a,0))){
execute(child(a,1));
}
}else if (a->kind == "WHILE") {
while(boolean_evaluation(child(a,0))){
execute(child(a,1));
}
}else if(a->kind == "="){
dps[child(a,0)->text]= list_evaluation(child(a,1));
}else if(a->kind == "list"){
for (int i = 0; child(a, i) != NULL; ++i) {
execute(child(a, i));
}
}else if(a->kind == "PLOT"){
list<pair<int,int> > temp = list_evaluation(child(a,0));
cout << "PLOT ";
draw_plot(temp);

    }else if(a->kind == "LOGPLOT"){
list<pair<int,int> > temp = list_evaluation(child(a,0));
list<pair<int,int> >::iterator it = temp.begin();
for(; it != temp.end(); ++it){
if((*it).second != 0)(*it).second = log((*it).second);
}
cout << "LOGPLOT";
draw_plot(temp);
}

}


int main() {
AST *root = NULL;
ANTLR(plots(&root), stdin);
ASTPrint(root);
execute(root);

}

  

void
#ifdef __USE_PROTOS
plots(AST**_root)
#else
plots(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  linterpretation(zzSTR); zzlink(_root, &_sibling, &_tail);
  (*_root)=createASTstring(_sibling,"DataPlotsProgram");
  zzmatch(1);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
linterpretation(AST**_root)
#else
linterpretation(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x2) ) {
      instruction(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTstring(_sibling,"list");
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

void
#ifdef __USE_PROTOS
instruction(AST**_root)
#else
instruction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==ID) ) {
    zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      if ( (setwd1[LA(1)]&0x8) ) {
        data(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (setwd1[LA(1)]&0x10) ) {
          function(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (setwd1[LA(1)]&0x20) ) {
      plot(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==WHILE) ) {
        while_bucle(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==IF) ) {
          if_statement(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
while_bucle(AST**_root)
#else
while_bucle(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  block(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDWHILE);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
if_statement(AST**_root)
#else
if_statement(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(IF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  block(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDIF);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
block(AST**_root)
#else
block(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(OPAR);  zzCONSUME;
  condition(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(TPAR);  zzCONSUME;
  linterpretation(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
condition(AST**_root)
#else
condition(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd2[LA(1)]&0x4) ) {
      }
      else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  orcondition(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
orcondition(AST**_root)
#else
orcondition(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  andcondition(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==OR) ) {
      zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      andcondition(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
andcondition(AST**_root)
#else
andcondition(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  opboolean(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==AND) ) {
      zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      opboolean(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x20);
  }
}

void
#ifdef __USE_PROTOS
opboolean(AST**_root)
#else
opboolean(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd2[LA(1)]&0x40) ) {
      comparation(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd2[LA(1)]&0x80) ) {
        boolfunction(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==OPAR) ) {
          zzmatch(OPAR);  zzCONSUME;
          condition(zzSTR); zzlink(_root, &_sibling, &_tail);
          zzmatch(TPAR);  zzCONSUME;
        }
        else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
comparation(AST**_root)
#else
comparation(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  value(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==EQUAL) ) {
      zzmatch(EQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==DIFF) ) {
        zzmatch(DIFF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==GREAT) ) {
          zzmatch(GREAT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==LESS) ) {
            zzmatch(LESS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  value(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
value(AST**_root)
#else
value(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd3[LA(1)]&0x4) ) {
      data(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==ITH) ) {
        datafunction(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}

void
#ifdef __USE_PROTOS
datafunction(AST**_root)
#else
datafunction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ITH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(OPAR);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COMA);  zzCONSUME;
  data(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(TPAR);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

void
#ifdef __USE_PROTOS
boolfunction(AST**_root)
#else
boolfunction(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==EMPTY) ) {
      zzmatch(EMPTY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==CHECK) ) {
        zzmatch(CHECK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(OPAR);  zzCONSUME;
  data(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(TPAR);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}

void
#ifdef __USE_PROTOS
function(AST**_root)
#else
function(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd3[LA(1)]&0x40) ) {
    pop_amend_normalize(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (LA(1)==PUSH) ) {
      push(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x80);
  }
}

void
#ifdef __USE_PROTOS
pop_amend_normalize(AST**_root)
#else
pop_amend_normalize(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    {
      zzBLOCK(zztasp3);
      zzMake0;
      {
      if ( (LA(1)==POP) ) {
        zzmatch(POP); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==AMEND) ) {
          zzmatch(AMEND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==NORMALIZE) ) {
            zzmatch(NORMALIZE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      zzEXIT(zztasp3);
      }
    }
    zzmatch(OPAR);  zzCONSUME;
    data(zzSTR); zzlink(_root, &_sibling, &_tail);
    zzmatch(TPAR);  zzCONSUME;
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x1);
  }
}

void
#ifdef __USE_PROTOS
push(AST**_root)
#else
push(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(PUSH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(OPAR);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd4[LA(1)]&0x2) ) {
      data(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd4[LA(1)]&0x4) ) {
        function(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(COMA);  zzCONSUME;
  points(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(TPAR);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x8);
  }
}

void
#ifdef __USE_PROTOS
plot(AST**_root)
#else
plot(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==PLOT) ) {
      zzmatch(PLOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==LOGPLOT) ) {
        zzmatch(LOGPLOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(OPAR);  zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd4[LA(1)]&0x10) ) {
      data(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (setwd4[LA(1)]&0x20) ) {
        function(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(TPAR);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x40);
  }
}

void
#ifdef __USE_PROTOS
data(AST**_root)
#else
data(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==OCLA) ) {
      dataset(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==ID) ) {
        zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {zzFAIL(1,zzerr13,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==CONC) ) {
      zzmatch(CONC);  zzCONSUME;
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==OCLA) ) {
          dataset(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==ID) ) {
            zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr14,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTstring(_sibling,"def");
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x80);
  }
}

void
#ifdef __USE_PROTOS
dataset(AST**_root)
#else
dataset(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(OCLA);  zzCONSUME;
  points(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==COMA) ) {
      zzmatch(COMA);  zzCONSUME;
      points(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzmatch(TCLA); 
  (*_root)=createASTstring(_sibling,"literal");
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x1);
  }
}

void
#ifdef __USE_PROTOS
points(AST**_root)
#else
points(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(GREAT);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(COMA);  zzCONSUME;
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LESS); 
  (*_root)=createASTstring(_sibling,"pair");
 zzCONSUME;

  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x2);
  }
}
