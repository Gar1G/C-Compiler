# C to ARM Assembly Compiler

## The aim of this project was to design a C compiler and generate functioning ARM assembly code. 

This compiler can be broken up into four distinct parts: lexer, parser, Abstract Syntax Tree (AST) and a code generator. More explicitly the steps performed by this compiler are:
1. Lexical Analysis
  * `compiler.l`: - breaks up input code into Tokens i.e operators, identifiers and numbers.
2. Syntax Analysis - Parser uses a set of grammar production rules to check token arrangements and determines whether the code is syntactically correct.
3. Semantic Analysis - After checking syntax, the parser then checks that expressions formed from the input token stream are semantically correct.
4. AST - Following semantically correct code, a tree representation of the input source code can be created. Using the grammar rules, only certain details of the source code a needed to be stored in the tree structure. 
5. ARM code generation - Using the tree representation of the input source code, equivalent ARM assembly instructions are generated and written to a .s file.

