%{

/*
 * scanner.flex -- SPL scanner specification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/errors.h>
#include <util/memory.h>
#include <table/identifier.h>
#include <types/types.h>
#include <absyn/absyn.h>
#include <phases/_01_scanner/scanner.h>
#include <phases/_02_03_parser/parser.h>

static Position currentPosition = {1, 1};

%}

 /* Disables the ability to read multiple files (which is not required in SPL). */
%option noyywrap
 /* Sets the caching behavior of the generated scanner. */
%option never-interactive
 /* This header may not always exist on Windows systems. */
%option nounistd
 /* Disables unused functions, so no warnings are displayed during compilation. */
%option nounput
%option noinput

%{
    CommandLineOptions *commandLineOptions = NULL;

    /**
     * Advances the global column counter (columnNumber) by the length of the most recently matched lexem (yytext) using yyleng.
     */
    static void advanceColumnCounter(){
        currentPosition.column += yyleng;
    }

    /**
     * Advances the current position counter to the start of the next line.
     * The line number is incremented and the column number is reset to 1.
     * Use this when detecting a new line to ensure correct token positions.
     */
    static void advancePositionToNextLine(){
        currentPosition.line++;
        currentPosition.column = 1;
    }

    /**
     * Creates a symbol with a line number and no value (NoVal).
     * Note: Also calls 'advanceColumnCounter' after using the current column number.
     *
     * @param type The token type that was detected (for example ARRAY).
     * @return The given token type.
     */
    static int symbol(int type) {
        yylval.noVal.position = currentPosition;
        advanceColumnCounter();
        return type;
    }

    /**
     * Creates a symbol with a line number and an Identifier* as value (IdentVal).
     * Note: Also calls 'advanceColumnCounter' after using the current column number.
     *
     * @param type The token type that was detected (should always be IDENT).
     * @param value The identifier that was detected. (Use newIdentifier(char* string) to construct an Identifier from yytext)
     * @return The given token type.
     */
    static int symbolIdentVal(int type, Identifier* value) {
        yylval.identVal.position = currentPosition;
        advanceColumnCounter();
        yylval.identVal.val = value;
        return type;
    }

    /**
     * Creates a symbol with a line number and an integer as value (IntVal).
     * Note: Also calls 'advanceColumnCounter' after using the current column number.
     *
     * @param type The token type that was detected (should always be INTLIT).
     * @param value The value of the integer literal that was detected.
     * @return The given token type.
     */
    static int symbolIntVal(int type, int value) {
        yylval.intVal.position = currentPosition;
        advanceColumnCounter();
        yylval.intVal.val = value;
        return type;
    }
%}

D = [0-9][0-9]*       //Decimal
H = [0-9A-Fa-f][0-9A-Fa-f]* //Hex

%%

    // TODO (assignment 1): The regular expressions for all tokens need to be defined here.

.|\n		    {illegalCharacter(currentPosition, yytext[0]);}

//Schluesselwoerter
if            return IF;
else          return ELSE;
while         return WHILE;
proc          return PROC;
var           return VAR;
ref           return REF;
of            return OF;
type          return TYPE;
array         return ARRAY;

//Operatoren
"<"             return LT;
"#"             return NE;
":="            return ASGN;
"+"             return PLUS;
"/"             return SLASH;
"*"             return STAR;
">"             return GT;
"<="            return LE;
"-"             return MINUS;
">="            return GE;
"="             return EQ;

//Klammern
"("             return LPAREN;
")"             return RPAREN;
"["             return LBRACK;
"]"             return RBRACK;
"{"             return LCURL;
"}"             return RCURL;

//Token-Wert
[a-zA-Z_][a-zA-Z0-9_]*      return IDENT;
(D|H)                       return INTLIT;

//Sonstiges
":"             return COLON;
";"             return SEMIC;

%%
