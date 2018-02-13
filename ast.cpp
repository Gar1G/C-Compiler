//
//  ast.cpp
//  
//
//  Created by Akshay  on 09/03/2015.
//
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Expr_node;
class var_dec_node;
class Statement_node;
vector<Expr_node*> expr_list;
vector<var_dec_node*> parameter_list;
vector<Statement_node*> statement_list;



//used for
class Statement_node{
    
};

class Expr_node{
    
};

class Double_Node : public Expr_node{
public:
    double val;
    Double_Node(double val): val(val){
    }
};

class Int_Node : public Expr_node{
public:
    int val;
    Int_Node(int val): val(val){
    }
};

class ID_node : public Expr_node{
public:
    string name;
    ID_node(name) : name(name){
    }
};

class Assign_node : public Expr_node{
public:
    ID_node* left;
    Expr_node* right;
    Assign_node(ID_node* left, Expr_node* right): left(left), right(right){
    }
};


class Expression : public Statement_node{
    Expr_node* expr;
    Expression(Expr_node* expr) : expr(expr){
    }
};


class Func_call : public Expr_node{
    ID_node* id;
    expr_list args;
    Func_call(ID_node id, expr_list args):ID_node(id), expr_list(args){
    }
    
};


class var_dec_node : public Statement_node{
    int type;
    ID_node* ID;
    Assign_node* assgn;
    
    var_dec_node(int type, ID_node* ID) : type(type), ID(ID){
    }
    var_dec_node(int type, Assign_node* assgn) : type(type), assgn(assgn){
    }
    
};

class fun_dec_node : public Statement_node{
    int type;
    ID_node* ID;
    parameter_list args;
    scope block;
    
    fun_dec_node(int type, ID_node* ID, parameter_list args, scope block) : type(type), ID(ID), args(args), block(block){
    }
};


class scope : public Expr_node{
    statement_list stmts;
    
    scope(){
    }
};


class Logic_op : public Expr_node{
    int op;
    Expr_node* left;
    Expr_node* right;
    
    Logic_op(Expr_node* left, int op, Expr_node* right) : left(left), op(op), right(right){
    }
};

class Arithmetic_op : public Expr_node{
    int op;
    Expr_node* left;
    Expr_node* right;
    
    Arithmetic_op(Expr_node* left, int op, Expr_node* right) : left(left), op(op), right(right){
    }
};

