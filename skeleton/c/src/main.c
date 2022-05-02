/*
 * main.c -- SPL compiler
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/errors.h>
#include <absyn/absyn.h>
#include "phases/_01_scanner/scanner.h"
#include <phases/_02_03_parser/parser.h>
#include <phases/_04a_tablebuild/tablebuild.h>
#include "phases/_04b_semant/procedurebodycheck.h"
#include "phases/_05_varalloc/varalloc.h"
#include "phases/_06_codegen/codegen.h"
#include <sys/stat.h>

static Position tokenPosition(int token) {
    switch (token) {
        case IDENT:
            return yylval.identVal.position;
        case INTLIT:
            return yylval.intVal.position;
        default:
            return yylval.noVal.position;
    }
}

static void printToken(FILE *out, int token) {
    Position pos = tokenPosition(token);

    fprintf(out, "TOKEN = %s", tokenName(token));

    if (token != 0) fprintf(out, " in line %d, column %d", pos.line, pos.column);

    switch (token) {
        case IDENT:
            fprintf(out, ", value = \"%s\"", yylval.identVal.val->string);
            break;
        case INTLIT:
            fprintf(out, ", value = %d", yylval.intVal.val);
            break;
        default:
            break; // Nothing to do
    }
    fprintf(out, "\n");
}

int main(int argc, char *argv[]) {
    CommandLineOptions *options = (CommandLineOptions *) malloc(sizeof(CommandLineOptions));
    parseCommandLineOptions(options, argc, argv);
    commandLineOptions = options;

    struct stat inFileStat;
    stat(options->inFileName, &inFileStat);

    yyin = fopen(options->inFileName, "r"); // Set input file for the scanner
    if (!S_ISREG(inFileStat.st_mode) || yyin == NULL) error("Cannot open input file '%s'", options->inFileName);

    if (options->phaseOption == PHASE_TOKENS) {
        int token;
        do {
            token = yylex();
            printToken(stdout, token);
        } while (token != 0);
        fclose(yyin);
        exit(0);
    }

    Program *program;

    yydebug = false;    // Change to true for detailed parsing output. Don't forget to change it back
    if (yyparse(&program) != 0) {
        fclose(yyin);   // Close file even if parsing fails.
        error("Failed to parse input!");
    }

    fclose(yyin);

    if (options->phaseOption == PHASE_PARSE) {
        printf("Input parsed successfully!\n");
        exit(0);
    }

    if (options->phaseOption == PHASE_ABSYN) {
        printProgram(stdout, 0, program);
        exit(0);
    }

    SymbolTable *globalTable = buildSymbolTable(program, options->phaseOption == PHASE_TABLES);
    if (options->phaseOption == PHASE_TABLES) exit(0);

    checkProcedures(program, globalTable);
    if (options->phaseOption == PHASE_SEMANT) {
        printf("No semantic errors found!\n");
        exit(0);
    }

    allocVars(program, globalTable, options->phaseOption == PHASE_VARS, options->ershovOptimization);
    if (options->phaseOption == PHASE_VARS) exit(0);

    FILE *outFile = getOutputFile(options);
    if (outFile == NULL) error("Cannot open output file '%s'", options->outFileName);

    genCode(program, globalTable, outFile, options->ershovOptimization);

    // Close outfile, but prevent closing stdout on accident
    if (outFile != stdout) fclose(outFile);
    return 0;
}
