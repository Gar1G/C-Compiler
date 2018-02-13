#ifndef AST_HPP
#define AST_HPP
#endif

#include <iostream>
#include <vector>
#include <string>
#include <map>

static int address = 0;
static bool regs[13] = {[0 ... 12] = true};


class ast_node{
public:
    virtual ~ast_node() {}
    virtual void Print(std::ostream &dst) const =0;
    virtual void codegen(std::map<std::string, int> &var_map, int &r, std::ostream &fst) const =0;
    virtual std::string get_node_type()const=0;
    
};


class Statement_node : public ast_node{
public:
    virtual void Print(std::ostream &dst)const =0;
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst) const =0;
    virtual std::string get_node_type()const=0;
    

};

class Expr_node: public ast_node {
public:
    
    virtual void Print(std::ostream &dst) const = 0;
    virtual std::string get_id() const=0;
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst) const =0;
    virtual std::string get_node_type()const=0;
};

class Double_Node : public Expr_node{
private:
    double val;
public:
    Double_Node(double d_val): val(d_val){
    }
    virtual void Print(std::ostream &dst) const {
        dst << "Double val " << val << std::endl;
    }
    virtual std::string get_id() const{
        return "NOT AN ID";
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        fst << val;
    }
    virtual std::string get_node_type()const{
        return "double";
    }

};

class Int_Node : public Expr_node{
private:
    int val;
public:
    Int_Node(int i_val): val(i_val){
    }
    virtual void Print(std::ostream &dst) const {
        dst << "Int val " << val << std::endl;
    }
    virtual std::string get_id() const{
        return "NOT AN ID";
    }
    
    virtual void codegen(std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        for(int i=0; i<13; i++){
            if(regs[i]==true){
                r=i;
                break;
            }
            
        }
        fst << "MOV R" << r << ", #" << val << std::endl;
        regs[r]=false;
    }
    virtual std::string get_node_type()const{
        return "integer";
    }
    
    

};

class String_Node : public Expr_node{
private:
    std::string val;
public:
    String_Node(std::string s_val):val(s_val){
    }
    virtual void Print(std::ostream &dst) const {
        dst << "String val " << val << std::endl;
    }
    virtual std::string get_id() const{
        return "NOT AN ID";
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
    }
    virtual std::string get_node_type()const{
        return "string";
    }
     
    
    

};

class ID_node : public Expr_node{
private:
    std::string name;
public:
    ID_node(const std::string id):name(id){
    }
    virtual void Print(std::ostream &dst) const {
        dst << "ID " << name << std::endl;
    }
    virtual std::string get_id()const{
        return name;
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        int addr = var_map.find(name)->second;

        for(int i=0; i<13; i++){
            if(regs[i]==true){
                r=i;
                break;
            }
            
        }
        fst << "LDR R" << r << ", 0x" << addr << std::endl;
        regs[r]=false;
         
    }
    virtual std::string get_node_type()const{
        return "identifier";
    }
    
};

class Assign_node : public Expr_node{
private:
    ID_node* left;
    Expr_node* right;
public:
    Assign_node(ID_node* id, Expr_node* expr): left(id), right(expr){
    }
    
    virtual void Print(std::ostream &dst) const {
        dst << "Assign " << std::endl;
        if(left != NULL){
            left->Print(dst);
        }
        
        if(right!=NULL){
            right->Print(dst);
        }
    }
    
    virtual std::string get_id()const{
        return left->get_id();
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        int reg1, reg2;
        
        if(right!=NULL && left!=NULL){
            left->codegen(var_map, r, fst);
            reg1 = r;
            right->codegen(var_map, r, fst);
            reg2 = r;
            regs[reg2] = true;
            fst << "MOV R"<<reg1<<", R"<<reg2 <<std::endl;
            
            fst << "STR R"<<reg1<<", 0x"<< var_map.find(left->get_id())->second << std::endl;
            regs[reg1] = true;
            r = reg1;
        
            
        }
    }
    virtual std::string get_node_type()const{
        return "assignment";
    }
    
};

class IncDec_node : public Expr_node{
private:
    int op;
    ID_node* left;
    Expr_node* right;
public:
    IncDec_node(ID_node* id, int op, Expr_node* expr): left(id), op(op), right(expr){
        
    }
    virtual void Print(std::ostream &dst) const {
        dst << "IncDec " << std::endl;
        if(left != NULL){
            left->Print(dst);
            if(right!=NULL){
                right->Print(dst);
            }
        }
    }
    virtual std::string get_id()const{
        return left->get_id();
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        int reg1, reg2;
        if(left!=NULL){
            switch (op){
                case 285:/* ++ */
                    left->codegen(var_map, r, fst);
                    reg1 = r;
                    fst << "ADD R"<<reg1<<", R"<<reg1 << ", #1" <<std::endl;
                    
                    fst << "STR R"<<reg1<<", 0x"<< var_map.find(left->get_id())->second << std::endl;
                    regs[reg1] = true;
                    r = reg1;
                    break;
                    
                case 286:/* -- */
                    left->codegen(var_map, r, fst);
                    reg1 = r;
                    fst << "SUB R"<<reg1<<", R"<<reg1 << ", #1" <<std::endl;
                    
                    fst << "STR R"<<reg1<<", 0x"<< var_map.find(left->get_id())->second << std::endl;
                    regs[reg1] = true;
                    r = reg1;
                    break;
                case 287:/* += */
                    if(right!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        fst << "ADD R"<<reg1<<", R"<<reg1 << ", #R" << reg2 << std::endl;
                        fst << "STR R"<<reg1<<", 0x"<< var_map.find(left->get_id())->second << std::endl;
                        regs[reg1] = true;
                        r = reg1;
                        break;
                    }
                case 288:/* -= */
                    if(right!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        fst << "SUB R"<<reg1<<", R"<<reg1 << ", #R" << reg2 << std::endl;
                        fst << "STR R"<<reg1<<", 0x"<< var_map.find(left->get_id())->second << std::endl;
                        regs[reg1] = true;
                        r = reg1;
                        break;
                    }
            }
        }
        
    }
    
    virtual std::string get_node_type()const{
        return "incdec";
    }
};



typedef std::vector<Expr_node*> expressions;
class Func_call : public Expr_node{
private:
    ID_node* id;
    expressions expr_list;
public:
    Func_call(ID_node* funct_name, expressions exprs):id(funct_name), expr_list(exprs){
    }
    virtual void Print(std::ostream &dst) const {
        dst << "Funct call " << std::endl;
        if(id !=NULL){
            id ->Print(dst);
            dst << "[ ";
            for(int i=0; i<expr_list.size(); i++){
                if(i!=0){
                    dst << ",";
                }
                expr_list[i]->Print(dst);
            }
            dst << " ]";
        }
        
    }
    virtual std::string get_id()const{
        return id->get_id();
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        fst << "B " << id->get_id() << std::endl;
    }
    virtual std::string get_node_type()const{
        return "function call";
    }
    
    
};


class parameter: public Expr_node{
private:
    int type;
    ID_node* id;
public:
    parameter(int var_type, ID_node* name): type(var_type), id(name){
    }
    
    virtual void Print(std::ostream &dst) const{
        dst << "Parameter: type " << type << "  " << std::endl;
        if(id!=NULL){
            id->Print(dst);
        }
    }
    virtual std::string get_id()const{
        return id->get_id();
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
    }
    virtual std::string get_node_type()const{
        return "parameter";
    }
    
    
    
};


class Op_node: public Expr_node{
private:
    int op;
    Expr_node* left;
    Expr_node* right;
public:
    Op_node(Expr_node* left_exp, int op, Expr_node* right_expr): left(left_exp), op(op), right(right_expr){
    }
    
    virtual void Print(std::ostream &dst) const {
        dst << "Operation: " << std::endl;
        if(left != NULL){
            left->Print(dst);
            dst << "Op type: " << op;
            if(right!=NULL){
                right->Print(dst);
            }
        }
    }
    virtual std::string get_id() const{
        return "NO ID TO BE RETURNED";
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        int reg1, reg2, reg3, reg4;
        if(left!=NULL && right!=NULL){
            switch(op){
                case 275: /* + */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "ADD R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    
                    break;
            
                case 276:/* - */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "SUB R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    
                    break;
            
                case 278:/* * */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "MUL R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    
                    break;
                
                case 277:/* / */
                    fst << "DIV";
                    break;
                
                case 281:/* && */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        for(int i=0; i<13; i++){
                            if(regs[i]==true){
                                reg3=i;
                                break;
                            }
                        }
                        regs[reg3] = false;
                        fst << "CMP R"<<reg1 << "#0"<<std::endl;
                        fst << "BEQ label0" << std::endl;
                        fst << "CMP R"<<reg2 << "#0" << std::endl;
                        fst << "BEQ label0" << std::endl;
                        fst << "MOV R" << reg3 << ", #1"<< std::endl;
                        fst << "B label1" << std::endl;
                        for(int i=0; i<13; i++){
                            if(regs[i]==true){
                                reg4=i;
                                break;
                            }
                        }
                        regs[reg4] = false;
                        fst<<"label 0"<<std::endl;
                        fst << "MOV R"<<reg3<< ", #0"<<std::endl;
                        fst<<"label 1"<<std::endl;
                        fst << "MOV R"<<reg4<< ", R"<< reg3 << std::endl;
                        
                        r = reg4;
                        regs[reg1] = true;
                        regs[reg2] = true;
                        
                    }
                    
                    break;
                
                case 282:/* || */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        for(int i=0; i<13; i++){
                            if(regs[i]==true){
                                reg3=i;
                                break;
                            }
                        }
                        regs[reg3] = false;
                        fst << "CMP R"<<reg1 << "#0"<<std::endl;
                        fst << "BNE label0" << std::endl;
                        fst << "CMP R"<<reg2 << "#0" << std::endl;
                        fst << "BNE label0" << std::endl;
                        fst << "MOV R" << reg3 << ", #1"<< std::endl;
                        fst << "B label1" << std::endl;
                        for(int i=0; i<13; i++){
                            if(regs[i]==true){
                                reg4=i;
                                break;
                            }
                        }
                        regs[reg4] = false;
                        fst<<"label 0"<<std::endl;
                        fst << "MOV R"<<reg3<< ", #0"<<std::endl;
                        fst<<"label 1"<<std::endl;
                        fst << "MOV R"<<reg4<< ", R"<< reg3 << std::endl;
                        
                        r = reg4;
                        regs[reg1] = true;
                        regs[reg2] = true;
                        
                    }
                    
                    break;
                    
                case 283:/* == */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "SUBS R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        fst << "BEQ THENPART" << std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    
                    break;
                case 284:/* != */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "SUBS R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        fst << "BNE THENPART" << std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    break;
                case 279:/* < */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "SUBS R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        fst << "BLT THENPART" << std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    break;
                case 280:/* > */
                    if(right!=NULL && left!=NULL){
                        left->codegen(var_map, r, fst);
                        reg1 = r;
                        right->codegen(var_map, r, fst);
                        reg2 = r;
                        
                        fst << "SUBS R"<< reg1 <<", R"<<reg1<<", R"<<reg2<<std::endl;
                        fst << "BGT THENPART" << std::endl;
                        r = reg1;
                        regs[reg2]=true;
                    }
                    break;
                    
            }
        }
    }
    virtual std::string get_node_type()const{
        return "operation";
    }
    
    
    
    
};



class Expression : public Statement_node{
private:
    Expr_node* expr;
public:
    Expression(Expr_node* expr) : expr(expr){
    }
    virtual void Print(std::ostream &dst) const {
        dst << "EXPR " << std::endl;
        if(expr!=NULL){
            expr->Print(dst);
        }
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        expr->codegen(var_map, r, fst);
    }
    virtual std::string get_node_type()const{
        return "expression";
    }
    
};

class Scope_node : public Statement_node{
private:
    std::vector<Statement_node*> statement_list;
    
public:
    Scope_node(){
    }
    void Add(Statement_node* stmt){
        statement_list.push_back(stmt);
    }
    virtual void Print(std::ostream &dst) const {
        dst << "Scope " << std::endl;
        dst << "[ ";
        for(int i=0; i<statement_list.size(); i++){
            if(i!=0){
                dst << ",";
            }
            statement_list[i]->Print(dst);
        }
        dst << " ]";
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        for(int i=0; i<statement_list.size(); i++){
            statement_list[i]->codegen(var_map, r, fst);
        }
    }
    virtual std::string get_node_type()const{
        return "scope";
    }
    
    
};



class var_dec_node : public Statement_node{
private:
    int type;
    expressions expr_list;
public:
    
    var_dec_node(int var_type, expressions init_expr) : type(var_type), expr_list(init_expr){
    }
    
    virtual void Print(std::ostream &dst) const {
        dst << "VAR DEC " << type << std::endl;
        for(int i=0; i<expr_list.size(); i++){
            if(i!=0){
                dst << ",";
            }
            expr_list[i]->Print(dst);
        }
        dst << " ]";
    }
    std::string get_var_names(int i){
        return expr_list[i]->get_id();
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        for(int i=0; i<expr_list.size(); i++){
            //needs completing
            std::string name = expr_list[i]->get_id();
            
            if(var_map.find(name)!=var_map.end()){
            }
            else{
                var_map[name] = address;
                address += 4;
            }
            if(expr_list[i]->get_node_type() == "assignment"){
                expr_list[i]->codegen(var_map, r, fst);
            }
        }

    }
    virtual std::string get_node_type()const{
        return "variable declaration";
    }
    
    
};

typedef std::vector<parameter*> parameters;

class fun_dec_node : public Statement_node{
private:
    int type;
    ID_node* ID;
    parameters plist;
    Scope_node* block;
    
public:
    
    fun_dec_node(const int type, ID_node* ID, parameters params, Scope_node* block) : type(type), ID(ID), plist(params), block(block){
        
    }
    std::string get_fun_name(){
        return ID->get_id();
    }
    
    
    virtual void Print(std::ostream &dst) const {
        
        if(ID !=NULL){
            ID->Print(dst);
            dst << "[ ";
            for(int i=0; i<plist.size(); i++){
                if(i!=0){
                    dst << ",";
                }
                plist[i]->Print(dst);
            }
            dst << " ]";
            dst << "FUNCT DEC " << type << std::endl;
            if(block!=NULL){
                block->Print(dst);
            }
        }
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        fst << ID->get_id() <<": " << std::endl;
        if(block!=NULL){
            block->codegen(var_map, r, fst);
        }
    }
    
    virtual std::string get_node_type()const{
        return "function declaration";
    }
    
    
    
};


class if_node: public Statement_node{
private:
    Expr_node* left;
    Statement_node* middle;
    Statement_node* right;
public:
    
    if_node(Expr_node* condition, Statement_node* stmt, Statement_node* else_stmt): left(condition), middle(stmt), right(else_stmt){
    }
    
    virtual void Print(std::ostream &dst) const{
        dst << "If statement: " << std::endl;
        if(left!=NULL){
            left->Print(dst);
            if(middle!=NULL){
                dst << "do: ";
                middle->Print(dst);
                if(right!=NULL){
                    dst << "Else " << std::endl;
                    right->Print(dst);
                }
            }
        }
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
        if(left!=NULL){
            left->codegen(var_map, r, fst);
            if(right!=NULL){
                right->codegen(var_map, r, fst);
                fst << "B ENDCODE" << std::endl;
            }
            if(middle!=NULL){
                fst << "THENPART ";
                middle->codegen(var_map, r, fst);
                fst << "ENDCODE";
            }
        }

        
        
    }
    
    virtual std::string get_node_type()const{
        return "if statement";
    }
     
    
};

class while_loop: public Statement_node{
private:
    Expr_node* expression;
    Scope_node* block;
    
public:
    while_loop(Expr_node* exp, Scope_node* scope):expression(exp), block(scope){
    }
    
    virtual void Print(std::ostream &dst) const{
        dst << "While loop: " << std::endl;
        if(expression!=NULL){
            expression->Print(dst);
            if(block!=NULL){
                block->Print(dst);
            }
        }
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst) const{
    }
    virtual std::string get_node_type()const{
        return "While loop";
    }
    
    
    
};

class return_node: public Statement_node{
private:
    Expr_node* expr;
public:
    return_node(Expr_node* return_exp): expr(return_exp){
    }
    virtual void Print(std::ostream &dst) const{
        if(expr !=NULL){
            dst << "RETURN ";
            expr->Print(dst);
        }
    }
    
    virtual void codegen( std::map<std::string, int> &var_map, int &r, std::ostream &fst)const{
    }
    virtual std::string get_node_type()const{
        return "return statement";
    }
    
};



