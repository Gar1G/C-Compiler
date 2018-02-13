%option noyywrap
%{
#include "compiler.tab.hpp"
#include <iostream>
using namespace std;
%}

DIGIT [0-9]
ID [a-zA-Z_][a-zA-Z0-9_]*

	/*
	[ \t\n]						;
	{DIGIT}+					{yylval.int_val = atoi(yytext); return TINTEGER;}
	{DIGIT}+\.{DIGIT}+			{yylval.d_val = atof(yytext); return TD; }
	\=\=						{return TEQUAL;}
	\/\/(.)*					{return TLINEC;}
	\/\*[\s\S]*\*\/				{return TBLOCKC;}
	int 						{return TINT;}
	double						{return TDOUBLE;}
	float						{return TFLOAT;}
	char						{return TCHAR;}
	\=							{return TASSIGN;}
	\(							{return TOPA;}
	\)							{return TCPA;}
	\{							{return TOB;}
	\}							{return TCB;}
	\,							{return TCOMMA;}
	\;							{return TSEMIC;}
	if							{return TIF;}
	else						{cout << "TELSE" ;}
	for							{return TFOR;}
	printf						{cout << "TPRINT" ;}
	\+							{return TPLUS;}
	\-							{return TMINUS;}
	\/							{return TDIVIDE;}
	\*							{return TMULT;}
	\#include					{return TINCLUDE;}
	\<\s*[a-z]+.h\s*\>			{return TLIBRARY  ;}
	\%(d|i)						{return INTSPECIF;}
	\%f							{return FLOATSPECIF ;}
	\%c							{return CHARSPECIF ;}
	\%s							{return STRINGSPECIF ;}
	{ID}						{yylval.string = strdup(yytext); return TID;}
	*/
%%

[ \t\n]						;
{DIGIT}+\.{DIGIT}+			{cout << "double: " << yytext;}
{DIGIT}+					{cout << "integer: " << yytext;}
\=\=						{cout << "TEQUAL";}
\/\/(.)*					{cout << "TLINEC" ;}
\/\*[\s\S]*\*\/				{cout << "TBLOCKC" ;}
int 						{cout << "TINT" ;}
double						{cout << "TD" ;}
float						{cout << "TFLOAT";}
char						{cout << "TCHAR";}
\=							{cout << "TASSIGN";}
\(							{cout << "TOPA" ;}
\)							{cout << "TCPA" ;}
\{							{cout << "TOB" ;}
\}							{cout << "TCB" ;}
\,							{cout << "TCOMMA" ;}
\;							{cout << "TSEMIC" ;}
if							{cout << "TIF" ;}
else						{cout << "TELSE" ;}
for							{cout << "TFOR" ;}
printf						{cout << "TPRINT" ;}
\+							{cout << "TPLUS" ;}
\-							{cout << "TMINUS" ;}
\/							{cout << "TDIV" ;}
\*							{cout << "TMULT" ;}
\#include					{cout << "TINCLUDE" ;}
\<\s*[a-z]+.h\s*\>			{cout << "TLIBRARY" << yytext ;}
\%(d|i)						{cout << "INTSPECIF ;}
\%f							{cout << "FLOATSPECIF ;}
\%c							{cout << "CHARSPECIF ;}
\%s							{cout << "STRINGSPECIF ;}
{ID}						{cout << "identifier: " << yytext ;}

%%

int main()
{
    yyFlexLexer lexer;
    while(lexer.yylex() != 0);

    return 0;
}
