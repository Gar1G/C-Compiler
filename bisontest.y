%code requires{
    
    #include <iostream>
    #include <stdio.h>
    #include <fstream>
    #include "ast.hpp"
    extern Scope_node *blob;
    
    void newfunction(std::string name);
    void newvariable(std::string name);
    void function_use(std::string name);
    void variable_use(std::string name);
    void codegen(Scope_node* root);
    
    int yylex();
    
    void yyerror(const char *s);
    extern int yyparse();
}


%{
    #include <map>
    std::map<std::string, bool> function;
    std::map<std::string, bool> variables;
%}

%union{
    ast_node * node;
    Expr_node *e_node;
    Statement_node* stmt;
    var_dec_node* vardec;
    Assign_node* assign;
    IncDec_node* incdec;
    fun_dec_node* fundec;
    parameter* p;
    Scope_node * blk;
    String_Node* s;
    std::vector<Expr_node*> *expressions;
    std::vector<var_dec_node*> *variable_decs;
    std::vector<ID_node*> *ids;
    std::vector<parameter*> *params;
    ID_node* ident;
    Int_Node* integer;
    Double_Node* dbl;
    int int_val;
    double d_val;
    std::string *strng;
    int token;
    
}

%token TLINEC TBLOCKC TPRINT TRETURN TWHILE
%token TASSIGN TCPA TOB TCB TCOMMA TSEMIC
%token TIF TELSE
%token<token> TINT TFLOAT TD TCHAR
%token<token> TPLUS TMINUS TDIV TMULT
%token<token> TLT TGT TAND TOR TEQUAL TNE TINC  TDEC  TE_INC  TE_DEC
%token<int_val> TINTEGER
%token<d_val> TDOUBLE
%token<strng> TID TSTRING
%token TOPA TFOR
%right TELSE


%type<blk>  program command_list scope local_dec_list
%type<stmt> declaration stmt return_stmt if_stmt localdec loop
%type<fundec> function_dec
%type<vardec> variable_dec
%type<e_node> expression  function_call  num v_dec factor simple_expr term sum_expr and_expr rel_expr
%type<token> sum_op mul_op logical_op type
%type<ident> identifier
%type<expressions> var_list var_dec_list
%type<params> p_list
%type<assign> assignment
%type<incdec> incdec
%type<p> param
%type<s> string




%start program

%%

/* using left recursion */

program : command_list {blob = $1;}

;

command_list : declaration {$$ = new Scope_node(); $$->Add($1); std::cout << "Declaration " << std::endl;}
| command_list declaration {$1->Add($2);}
;

declaration : function_dec {$<fundec>$ = $1; std::cout << "Funct. Dec " << std::endl;}
| variable_dec  {$<vardec>$ = $1; std::cout << "Var Dec " << std::endl;}
| expression  TSEMIC {$$ = new Expression($1); std::cout << "expr" << std::endl;}
;


variable_dec : type var_dec_list TSEMIC  {$$ = new var_dec_node($1, *$2);
    for(int i=0; i<$2->size(); i++){
        newvariable($$->get_var_names(i));
    }
    std::cout << "var list size; " << $2->size();
    std::cout << "variable declared" << std::endl;}
;

var_dec_list : v_dec {$$ = new expressions(); $$->push_back($1);}
| var_dec_list TCOMMA v_dec {$1->push_back($3);}
;

v_dec : identifier {$<ident>$ = $1;}  | assignment {$<assign>$ = $1;}
;


assignment : identifier TASSIGN expression {$$ = new Assign_node($1, $3); std::cout << "assigned" << std::endl;}

;

    
function_dec : type identifier TOPA p_list TCPA scope {$$ = new fun_dec_node($1, $2, *$4, $6); newfunction($$->get_fun_name());}
| type identifier TOPA p_list TCPA TSEMIC {$$ = new fun_dec_node($1, $2, *$4, NULL); newfunction($$->get_fun_name());}
;


p_list : param {$$ = new parameters(); $$->push_back($1);}
| p_list TCOMMA param {$1->push_back($3);}
| {$$ = new parameters();}
;

param : type identifier {$$ = new parameter($1, $2);}
;


type : TINT {$$=$1; std::cout << "INT type" << std::endl;}
| TD {$$=$1;}
    | TFLOAT {$$=$1;}
    | TCHAR {$$=$1;}
;

/* inside function scope */

scope : TOB local_dec_list TCB {$$=$2; std::cout << "scope" << std::endl;}
| TOB TCB {$$ = new Scope_node(); std::cout << "emtpy scope" << std::endl;}
    
;

local_dec_list : localdec {$$ = new Scope_node(); $$->Add($1); std::cout << "Declaration " << std::endl;}
| local_dec_list localdec {$1->Add($2);}

localdec : declaration | stmt

stmt : scope {$<blk>$ = $1;}
| return_stmt
| if_stmt
| loop
;


loop : TWHILE TOPA simple_expr TCPA scope {$$= new while_loop($3, $5);}
;

return_stmt : TRETURN expression TSEMIC {$$ = new return_node($2); std::cout << "Return statement" << std::endl;}
;



expression: assignment {$<assign>$ = $1;}
| incdec {$<incdec>$ = $1;}
| simple_expr
;
simple_expr : and_expr
| simple_expr TOR and_expr {$$ = new Op_node($1, $2, $3);}
;
and_expr : rel_expr
| and_expr TAND rel_expr {$$ = new Op_node($1, $2, $3);}
;
rel_expr : sum_expr logical_op sum_expr {$$ = new Op_node($1, $2, $3);}
| sum_expr
;
sum_expr : term
| sum_expr sum_op term {$$ = new Op_node($1, $2, $3);}
;
term : factor
| term mul_op factor {$$ = new Op_node($1, $2, $3);}
;
factor : function_call
| TOPA expression TCPA {$$ = $2;}
| num
| string {$<s>$ = $1;}
| identifier {$<ident>$ = $1; variable_use($1->get_id());}
;

incdec : identifier TE_INC expression {$$ = new IncDec_node($1, $2, $3);}
| identifier TE_DEC expression {$$ = new IncDec_node($1, $2, $3);}
| identifier TINC {$$ = new IncDec_node($1, $2, NULL);}
| identifier TDEC {$$ = new IncDec_node($1, $2, NULL);}
;



if_stmt : TIF TOPA expression TCPA stmt %prec TELSE {$$ = new if_node($3, $5, NULL);}
| TIF TOPA expression TCPA stmt TELSE stmt {$$ = new if_node($3, $5, $7);}

;
function_call : identifier TOPA var_list TCPA {$$ = new Func_call($1, *$3); function_use($1->get_id()); std::cout << "function call successful" << std::endl;}
;

var_list : expression {$$ = new expressions(); $$->push_back($1);}
| var_list TCOMMA expression {$1->push_back($3);}
| {$$ = new expressions(); std::cout << "no variables" << std::endl;}
;



identifier : TID {$$ = new ID_node(*$1); std::cout << "identifier found" << std::endl;}
;

string : TSTRING {$$ = new String_Node(*$1);}
;

num : TINTEGER {$$ = new Int_Node($1); std::cout << "found an number int" << std::endl;}
| TDOUBLE {$$ = new Double_Node($1);}
;

sum_op : TPLUS | TMINUS
;

mul_op : TDIV | TMULT
;
logical_op : TLT | TGT  | TEQUAL | TNE
;



%%
Scope_node* blob;

int main(int, char**){
    
    yyparse();
    
    Scope_node *root=blob;
    
    root->Print(std::cout);
    codegen(blob);
    function.clear();
    variables.clear();
    
    
    
    return 0;
}

void newfunction(std::string name){
    if(function.find(name)!=function.end()){
        std::cout << "Semantic Error: Function name: " << name << " already declared" << std::endl;
        exit(-1);
    }
    else{
        function[name]=true;
    }
}

void newvariable(std::string name){
    if(variables.find(name)!=variables.end()){
        std::cout << "Semantic Error: Variable name:" << name <<  " already declared" << std::endl;
        exit(-1);
    }
    else{
        variables[name]=true;
    }
    
}

void function_use(std::string name){
    if(function.find(name)!=function.end() || name == "printf" || name == "exit"){
        
    }
    else{
        std::cout << "Semantic Error: No declaration for function name: " << name << std::endl;
        exit(-1);
    }
}

void variable_use(std::string name){
    if(variables.find(name)!=variables.end()){
        
    }
    else{
        std::cout << "Semantic Error: No declaration for variable name: " << name << std::endl;
        exit(-1);
    }
}


void codegen(Scope_node* root){
    std::map<std::string, int> var_regs;
    int reg=0;
    
    std::ofstream outfile;
    outfile.open("arm_out.s");
    if(!outfile.is_open()){
        std::cout<<"ERROR: Could not open file" << std::endl;
    }
    root->codegen(var_regs, reg, outfile);
    
}




void yyerror(const char *s){
    std::cout << "Parse error: " << s << std::endl;
    exit(-1);
}



