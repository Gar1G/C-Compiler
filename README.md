# C to ARM Assembly Compiler

## The aim of this project was to design a C compiler and generate functioning ARM assembly code. 

This compiler can be broken up into four distinct parts: lexer, parser, Abstract Syntax Tree (AST) and a code generator. More explicitly the steps performed by this compiler are:
1. `compiler.l`
 * Lexical Analysis : Lexer breaks up input code into tokens i.e operators, identifiers and numbers.
2. `bisontest.y`
 * Syntax Analysis : Parser uses a set of grammar production rules to check token arrangements and determines whether the code is syntactically correct.
 * Semantic Analysis - After checking syntax, the parser then checks that expressions formed from the input token stream are semantically correct.
3. `ast.hpp`
 * AST : a tree representation of the input source code can be created. Using the grammar rules, only certain details of the source code a needed to be stored in the tree structure.
 * Code Generation : Using the tree representation of the input source code, equivalent ARM assembly instructions are generated and written to a .s file.


