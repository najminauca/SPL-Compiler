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
%expect 1 //TODO Change?
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
%token	<noVal>		REF TYPE VAR WHILE DO
%token	<noVal>		LPAREN RPAREN LBRACK
%token	<noVal>		RBRACK LCURL RCURL
%token	<noVal>		EQ NE LT LE GT GE
%token	<noVal>		ASGN COLON COMMA SEMIC
%token	<noVal>		PLUS MINUS STAR SLASH
%token	<identVal>	IDENT
%token	<intVal>	INTLIT

%type   <expression>            exp rel_exp add_exp mul_exp unary_exp primary_exp
%type   <variable>              variable
%type   <statement>             stm empty_stm assign_stm compound_stm if_stm while_stm call_proc
%type   <typeExpression>        type
%type   <globalDeclaration>     global_dec proc_dec global_var
%type   <variableDeclaration>   local_var_dec
%type   <parameterDeclaration>  par_dec

%type   <statementList>         stm_list
%type   <expressionList>        arg_list
%type   <variableList>          local_var_list
%type   <parameterList>         par_list non_empty_par
%type   <globalDeclarationList> program global_list
%type   <intVal>                array_index

%start			program

%%

program             : global_list { *program = newProgram(newPosition(0,0),$1); }
                    ;

global_list         : { $$ = emptyGlobalDeclarationList(); }
                    | global_dec global_list { $$ = newGlobalDeclarationList($1,$2); }
                    ;

type                : IDENT { $$ = newNamedTypeExpression($1.position,$1.val); }
                    | ARRAY array_index OF type { $$ = newArrayTypeExpression($1.position, $2.val, $4); }
                    ;
global_dec          : global_var { $$ = $1; }
                    | proc_dec { $$ = $1; }
                    ;
global_var          : TYPE IDENT EQ type SEMIC { $$ = newTypeDeclaration($1.position,$2.val,$4); }
                    ;
proc_dec            : PROC IDENT LPAREN par_list RPAREN LCURL local_var_list stm_list RCURL { $$ = newProcedureDeclaration($1.position,$2.val,$4,$7,$8); }

par_list            : { $$ = emptyParameterList(); }
                    | non_empty_par { $$ = $1; }
                    ;
non_empty_par       : par_dec { $$ = newParameterList($1,emptyParameterList()); }
                    | par_dec COMMA non_empty_par { $$ = newParameterList($1,$3); }
                    ;
par_dec             : REF IDENT COLON type { $$ = newParameterDeclaration($1.position,$2.val,$4,true); }
                    | IDENT COLON type { $$ = newParameterDeclaration($1.position,$1.val,$3,false); }
                    ;

local_var_list      : { $$ = emptyVariableList(); }
                    | local_var_dec local_var_list { $$ = newVariableList($1,$2); }
                    ;
local_var_dec       : VAR IDENT COLON type SEMIC { $$ = newVariableDeclaration($1.position,$2.val,$4); }
                    ;

stm_list            : { $$ = emptyStatementList(); }
                    | stm stm_list { $$ = newStatementList($1,$2); }
                    ;

stm                 : empty_stm { $$ = $1; }
                    | assign_stm { $$ = $1; }
                    | compound_stm { $$ = $1; }
                    | if_stm { $$ = $1; }
                    | while_stm { $$ = $1; }
                    | call_proc { $$ = $1; }
                    ;

empty_stm           : SEMIC { $$ = newEmptyStatement($1.position); }
                    ;
compound_stm        : LCURL stm_list RCURL { $$ = newCompoundStatement($1.position, $2); }
                    ;
if_stm              : IF LPAREN exp RPAREN stm { $$ = newIfStatement($1.position,$3,$5,newEmptyStatement($1.position)); }
                    | IF LPAREN exp RPAREN stm ELSE stm { $$ = newIfStatement($1.position,$3,$5,$7); }
                    ;
while_stm           : WHILE LPAREN exp RPAREN stm { $$ = newWhileStatement($1.position,$3,$5); }
                    | DO stm WHILE LPAREN exp RPAREN { $$ = newWhileStatement($1.position,$5,$2); }
                    ;
assign_stm          : variable ASGN exp SEMIC { $$ = newAssignStatement($1->position,$1,$3); }
                    ;
call_proc           : IDENT LPAREN RPAREN SEMIC { $$ = newCallStatement($1.position,$1.val,emptyExpressionList()); }
                    | IDENT LPAREN arg_list RPAREN SEMIC { $$ = newCallStatement($1.position,$1.val,$3); }
                    ;

arg_list            : exp { $$ = newExpressionList($1,emptyExpressionList()); }
                    | exp COMMA arg_list { $$ = newExpressionList($1,$3); }
                    ;

exp                 : rel_exp { $$ = $1; }
                    ;
rel_exp             : add_exp { $$ = $1; }
                    | add_exp EQ add_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_EQU,$1,$3); }
                    | add_exp NE add_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_NEQ,$1,$3); }
                    | add_exp LT add_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_LST,$1,$3); }
                    | add_exp LE add_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_LSE,$1,$3); }
                    | add_exp GT add_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_GRT,$1,$3); }
                    | add_exp GE add_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_GRE,$1,$3); }
                    ;
add_exp             : mul_exp { $$ = $1; }
                    | add_exp PLUS mul_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_ADD,$1,$3); }
                    | add_exp MINUS mul_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_SUB,$1,$3); }
                    ;
mul_exp             : unary_exp { $$ = $1; }
                    | mul_exp STAR unary_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_MUL,$1,$3); }
                    | mul_exp SLASH unary_exp { $$ = newBinaryExpression($1->position,ABSYN_OP_DIV,$1,$3); }
                    ;
unary_exp           : primary_exp { $$ = $1; }
                    | MINUS unary_exp { $$ = newBinaryExpression($1.position,ABSYN_OP_SUB,newIntLiteral($1.position,0),$2); }
                    ;
primary_exp         : INTLIT { $$ = newIntLiteral($1.position, $1.val); }
                    | variable { $$ = newVariableExpression($1->position,$1); }
                    | LPAREN rel_exp RPAREN { $$ = $2; }
                    ;

array_index         : LBRACK INTLIT RBRACK { $$ = $2; }
                    | LBRACK INTLIT RBRACK array_index { $$.val = $2.val*$4.val; }
                    ;

variable            : IDENT { $$ = newNamedVariable($1.position,$1.val); }
                    | variable LBRACK add_exp RBRACK { $$ = newArrayAccess($1->position,$1,$3); }
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
