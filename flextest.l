%option noyywrap
%{
#include <iostream>
#include <stdio.h>
#include "bisontest.tab.hh"
using namespace std;
int linenum =0;

%}


DIGIT [0-9]
ID [a-zA-Z_][a-zA-Z0-9_]*

%%
"=="						{yylval.token = TEQUAL;return TEQUAL;}
"+="						{yylval.token = TE_INC; return TE_INC;}
"-="						{yylval.token = TE_DEC; return TE_DEC;}
"++"						{yylval.token = TINC; return TINC;}
"--"						{yylval.token = TDEC; return TDEC;}
"int" 						{yylval.token = TINT; return TINT;}
"double"					{yylval.token = TDOUBLE; return TD;}
"float"						{yylval.token = TFLOAT;return TFLOAT;}
"char"						{yylval.token = TCHAR;return TCHAR;}
"return"					{yylval.token = TRETURN;return TRETURN;}
"if"						{return TIF;}
"else"						{yylval.token = TELSE;return TELSE;}
"for"						{return TFOR;}
"while"						{return TWHILE;}
"="							{yylval.token = TASSIGN;return TASSIGN;}
"("							{return TOPA;}
")"							{return TCPA;}
"{"							{return TOB;}
"}"							{return TCB;}
","							{return TCOMMA;}
";"							{return TSEMIC;}
"for"						{return TFOR;}
"+"							{yylval.token = TPLUS ;return TPLUS;}
"-"							{yylval.token = TMINUS;return TMINUS;}
"/"							{yylval.token = TDIV;return TDIV;}
"*"							{yylval.token = TMULT;return TMULT;}
{DIGIT}+\.{DIGIT}+			{yylval.d_val = atof(yytext); return TDOUBLE; }
{DIGIT}+					{yylval.int_val = atoi(yytext); return TINTEGER;}
["][^"]*["]					{yylval.strng = new std::string(yytext);return TSTRING;}
{ID}						{yylval.strng = new std::string(yytext); return TID;}
\n							{linenum++;}
[\t]						;
%%

