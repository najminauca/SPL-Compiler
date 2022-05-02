/*
 * scanner.h -- SPL scanner interface
 */

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <command_line_options.h>

typedef struct {
    Position position;
} NoVal;

typedef struct {
    Position position;
    int val;
} IntVal;

typedef struct {
    Position position;
    Identifier *val;
} IdentVal;

extern CommandLineOptions *commandLineOptions;

//Declare global flex variables so they are accessible
extern FILE *yyin;
extern char *yytext;

int yylex(void);

#endif /* _SCANNER_H_ */
