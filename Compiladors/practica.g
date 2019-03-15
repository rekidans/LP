
#header
<<
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
>>

<<
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

/// Returns the ith element in the dataset
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
                temp.pop_back();
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
        }else if(a->kind == "=="){
            int pos1 = atoi(child(child(a,0), 0)->kind.c_str()), pos2 = atoi(child(child(a,1), 0)->kind.c_str());
            list<pair<int,int> > temp1 = list_evaluation(child(child(a,0), 1)),  temp2 = list_evaluation(child(child(a,1), 1));
            pair<int, int> comp1 = get_ith(temp1, pos1), comp2 = get_ith(temp2, pos2);
            return comp1.first == comp2.first and comp1.second == comp2.second;

        }else if(a->kind == "!="){
            int pos1 = atoi(child(child(a,0), 0)->kind.c_str()), pos2 = atoi(child(child(a,1), 0)->kind.c_str());
            list<pair<int,int> > temp1 = list_evaluation(child(child(a,0), 1)),  temp2 = list_evaluation(child(child(a,1), 1));
            pair<int, int> comp1 = get_ith(temp1, pos1), comp2 = get_ith(temp2, pos2);
            return comp1.first != comp2.first or comp1.second != comp2.second;

        }else if(a->kind == ">"){
            int pos1 = atoi(child(child(a,0), 0)->kind.c_str()), pos2 = atoi(child(child(a,1), 0)->kind.c_str());
            list<pair<int,int> > temp1 = list_evaluation(child(child(a,0), 1)),  temp2 = list_evaluation(child(child(a,1), 1));
            pair<int, int> comp1 = get_ith(temp1, pos1), comp2 = get_ith(temp2, pos2);
            if(comp1.first == comp2.first) return comp1.second > comp2.second;
            else return comp1.first > comp2.first;

        }else if(a->kind == "<"){
            int pos1 = atoi(child(child(a,0), 0)->kind.c_str()), pos2 = atoi(child(child(a,1), 0)->kind.c_str());
            list<pair<int,int> > temp1 = list_evaluation(child(child(a,0), 1)),  temp2 = list_evaluation(child(child(a,1), 1));
            pair<int, int> comp1 = get_ith(temp1, pos1), comp2 = get_ith(temp2, pos2);
            if(comp1.first == comp2.first) return comp1.second < comp2.second;
            else return comp1.first < comp2.first;

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

>>

#lexclass START

#token ENDWHILE "ENDWHILE"
#token WHILE "WHILE"
#token ENDIF "ENDIF"
#token IF "IF"
#token LOGPLOT "LOGPLOT"
#token PLOT "PLOT"
#token NORMALIZE "NORMALIZE"
#token POP "POP"
#token PUSH "PUSH"
#token NOT "NOT"
#token AND "AND"
#token OR "OR"
#token CHECK "CHECK"
#token AMEND "AMEND"
#token ITH "ITH"
#token EMPTY "EMPTY"
#token NUM "[0-9]+"
#token ID "[0-9a-zA-Z]+"
#token ASSIG "\="
#token DIFF "!="
#token EQUAL "=="
#token COMA "\,"
#token CONC "\·"
#token OCLA "\["
#token TCLA "\]"
#token OPAR "\("
#token TPAR "\)"
#token GREAT "\<"
#token LESS "\>"
#token SPACE "[\ \n]" << zzskip();>>


plots:   linterpretation <<#0=createASTstring(_sibling,"DataPlotsProgram");>> "@"! ;
linterpretation: (instruction)*  <<#0=createASTstring(_sibling,"list");>>;
instruction:  ID ASSIG^ (data|function) | plot | while_bucle | if_statement;
//Conditionals and boolean expressions
while_bucle: WHILE^  block ENDWHILE!;
if_statement: IF^ block ENDIF!;
block: OPAR! condition TPAR! linterpretation;
condition: (NOT^ | ) orcondition;
orcondition: andcondition (OR^ andcondition)*;
andcondition: opboolean (AND^ opboolean)*;
opboolean: (comparation| boolfunction| OPAR! condition TPAR!);
comparation: value (EQUAL^ | DIFF^| GREAT^|LESS^) value;
//Functions
value: (data|datafunction);
datafunction: ITH^ OPAR! NUM COMA! data TPAR!;
boolfunction: (EMPTY^| CHECK^) OPAR! data TPAR!;
function: pop_amend_normalize | push;
pop_amend_normalize: ((POP^|AMEND^| NORMALIZE^)  OPAR! data TPAR!);
push: PUSH^ OPAR! (data|points|function) COMA! points TPAR!;
plot: (PLOT^|LOGPLOT^) OPAR! (data|function) TPAR!;
/// Data
data:  (dataset|ID) (CONC! (dataset | ID))*  <<#0=createASTstring(_sibling,"def");>>;
/// Dataset that form the data
dataset:  OCLA! points (COMA! points)* TCLA!  <<#0=createASTstring(_sibling,"literal");>>;
/// Points that form the dataset
points:  GREAT! NUM COMA! NUM LESS!  <<#0=createASTstring(_sibling,"pair");>>;
