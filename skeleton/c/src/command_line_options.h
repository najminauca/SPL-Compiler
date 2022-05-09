/*
 * command_line_options.h -- command line option utilities.
 */

#ifndef SPL_COMMAND_LINE_OPTIONS_H
#define SPL_COMMAND_LINE_OPTIONS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    PHASE_TOKENS, PHASE_PARSE, PHASE_ABSYN, PHASE_TABLES, PHASE_SEMANT, PHASE_VARS, PHASE_NONE
} PhaseOptions;

typedef struct {
    PhaseOptions phaseOption;
    bool ershovOptimization;
    bool doWhileEnabled;
    char *inFileName;
    char *outFileName;
} CommandLineOptions;

FILE *getOutputFile(CommandLineOptions *options);

void parseCommandLineOptions(CommandLineOptions *options, int argc, char *argv[]);

#endif //SPL_COMMAND_LINE_OPTIONS_H
