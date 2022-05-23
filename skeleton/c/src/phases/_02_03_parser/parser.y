%{

/*
 * parser.y -- SPL parser specification
 */

#include <stdlib.h>
#include <util/errors.h>
#include <table/identifier.h>
#include <types/types.h>
#include <absyn/absyn.h>
#include <phases/_01_scanner/scanner.h>
#include <phases/_02_03_parser/parser.h>

void yyerror(Program**, char *);

%}

// This section is placed into the header to make functions available to other modules
%code requires{
	/**
	  @return The name of the token class signalled by the given id.
	 */
	char const * tokenName(int token_class_id);
}

%token-table
%expect 0 //TODO Change?
%parse-param {Program** program}

%union {
  NoVal noVal;
  IntVal intVal;
  IdentVal identVal;

  Expression *expression;
  Variable *variable;
  Statement *statement;
  TypeExpression *typeExpression;
  GlobalDeclaration *globalDeclaration;
  VariableDeclaration *variableDeclaration;
  ParameterDeclaration *parameterDeclaration;

  StatementList *statementList;
  ExpressionList *expressionList;
  VariableDeclarationList *variableList;
  ParameterDeclarationList *parameterList;
  GlobalDeclarationList *globalDeclarationList;
}

%token	<noVal>		ARRAY ELSE IF OF PROC
%token	<noVal>		REF TYPE VAR WHILE
%token	<noVal>		LPAREN RPAREN LBRACK
%token	<noVal>		RBRACK LCURL RCURL
%token	<noVal>		EQ NE LT LE GT GE
%token	<noVal>		ASGN COLON COMMA SEMIC
%token	<noVal>		PLUS MINUS STAR SLASH
%token	<identVal>	IDENT
%token	<intVal>	INTLIT

%start			program

%%

program             : global_list
                    ;

global_list         :
                    | global_dec global_list
                    ;

type                : IDENT
                    ;

global_dec          : global_var
                    | proc_dec
                    ;

global_var          : TYPE IDENT EQ type
                    ;

proc_dec            : PROC IDENT LPAREN par_list RPAREN

par_list            :
                    | non_empty_par
                    ;

non_empty_par       : par_dec
                    | par_dec COMMA non_empty_par
                    ;

par_dec             : REF IDENT COLON type
                    ;

local_var_list      :
                    | local_var_dec local_var_list
                    ;

local_var_dec       : VAR IDENT COLON type SEMIC
                    ;

stm_list            :
                    | stm stm_list
                    ;

stm                 : empty_stm
                    | compound_stm
                    | assign_stm
                    | if_stm
                    | while_stm
                    ;

empty_stm           : SEMIC
                    ;

compound_stm        : LCURL stm_list RCURL
                    ;

if_stm              : IF LPAREN exp RPAREN LBRACK stm_list RBRACK
                    | IF LPAREN exp RPAREN LBRACK stm_list RBRACK ELSE LBRACK stm RBRACK
                    ;

while_stm           : WHILE LPAREN exp RPAREN LBRACK stm_list RBRACK

assign_stm          : variable ASGN exp SEMIC
                    ;

exp                 : rel_exp
                    ;

rel_exp             : add_exp
                    | add_exp EQ add_exp
                    | add_exp NE add_exp
                    | add_exp LT add_exp
                    | add_exp LE add_exp
                    | add_exp GT add_exp
                    | add_exp GE add_exp
                    ;

add_exp             : mul_exp
                    | add_exp PLUS mul_exp
                    | add_exp MINUS mul_exp
                    ;

mul_exp             : unary_exp
                    | mul_exp STAR unary_exp
                    | mul_exp SLASH unary_exp
                    ;

unary_exp           : primary_exp
                    | PLUS unary_exp
                    | MINUS unary_exp
                    ;

primary_exp         : INTLIT
                    ;

variable            : IDENT
                    ;
%%

void yyerror(Program** program, char *msg) {
    // The first parameter is needed because of '%parse-param {Program** program}'.
    // Both parameters are unused and gcc would normally emits a warning for them.
    // The following lines "use" the parameters, but do nothing. They are used to silence the warning.
    (void)program;
    (void)msg;
    syntaxError(yylval.noVal.position, yytext);
}

// This function needs to be defined here because yytname and YYTRANSLATE are only available in the parser's implementation file.
char const *tokenName(int token_class_id) {
  // 0 is a special case because token_name(token) return "$end" instead of EOF.
  return token_class_id == 0 ? "EOF" : yytname[YYTRANSLATE(token_class_id)];
}
