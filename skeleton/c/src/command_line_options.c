/*
 * command_line_options.c -- Implementation of command line option utilities.
 */

#include "command_line_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VERSION        "1.2"

void version() {
    printf("spl version %s (compiled %s)\n", VERSION, __DATE__);
}

void showUsage(FILE *out) {
    /* show some help how to use the program */
    fprintf(out, "Usage: spl [OPTION] INPUT_FILE [OUTPUT_FILE]\n");
    fprintf(out, "\n");
    fprintf(out, "Executes all compiler phases up to (and including) the specified one.\n");
    fprintf(out, "If no flag is specified, all phases are run and code is written to the output file.\n");
    fprintf(out, "Options:\n");
    fprintf(out, "  --tokens     Phase 1: Scans for tokens and prints them.\n");
    fprintf(out, "  --parse      Phase 2: Parses the stream of tokens to check for syntax errors.\n");
    fprintf(out, "  --absyn      Phase 3: Creates an abstract syntax tree from the input tokens and prints it.\n");
    fprintf(out, "  --tables     Phase 4a: Builds a symbol table and prints its entries.\n");
    fprintf(out, "  --semant     Phase 4b: Performs the semantic analysis.\n");
    fprintf(out, "  --vars       Phase 5: Allocates memory space for variables and prints the amount of allocated memory.\n");
    fprintf(out, "  --ershov     Enable register optimization using the ershov number.\n");
    fprintf(out, "  --doWhile    Enable the do-while statement.\n");
    fprintf(out, "  --version    Show compiler version.\n");
    fprintf(out, "  --help       Show this help.\n");
}

void usageError(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "Usage error: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    showUsage(stderr);
    exit(1);
}

FILE *getOutputFile(CommandLineOptions *options) {
    if (strlen(options->outFileName) == 0) return stdout;
    return fopen(options->outFileName, "w");
}

static void setPhaseOption(CommandLineOptions *options, PhaseOptions phase) {
    if (options->phaseOption != PHASE_NONE) usageError("More than one phase option specified.");
    options->phaseOption = phase;
}

void parseCommandLineOptions(CommandLineOptions *options, int argc, char **argv) {
    options->phaseOption = PHASE_NONE;
    options->ershovOptimization = false;
    options->doWhileEnabled = false;
    options->inFileName = "";
    options->outFileName = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--tokens") == 0) {
            setPhaseOption(options, PHASE_TOKENS);
        } else if (strcmp(argv[i], "--parse") == 0) {
            setPhaseOption(options, PHASE_PARSE);
        } else if (strcmp(argv[i], "--absyn") == 0) {
            setPhaseOption(options, PHASE_ABSYN);
        } else if (strcmp(argv[i], "--tables") == 0) {
            setPhaseOption(options, PHASE_TABLES);
        } else if (strcmp(argv[i], "--semant") == 0) {
            setPhaseOption(options, PHASE_SEMANT);
        } else if (strcmp(argv[i], "--vars") == 0) {
            setPhaseOption(options, PHASE_VARS);
        } else if (strcmp(argv[i], "--ershov") == 0) {
            options->ershovOptimization = true;
        } else if (strcmp(argv[i], "--doWhile") == 0) {
            options->doWhileEnabled = true;
        } else if (strcmp(argv[i], "--version") == 0) {
            version();
            exit(0);
        } else if (strcmp(argv[i], "--help") == 0) {
            showUsage(stdout);
            exit(0);
        } else {
            if (!(argv[i][0] == '-' && argv[i][1] == '-')) {
                if (strlen(options->inFileName) == 0) options->inFileName = argv[i];
                else if (strlen(options->outFileName) == 0) options->outFileName = argv[i];
                else usageError("Too many positional arguments!");
            } else {
                usageError("Unknown option '%s'!", argv[i]);
            }
        }
    }

    if (strlen(options->inFileName) == 0) usageError("No input file");
}
