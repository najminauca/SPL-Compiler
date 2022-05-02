#ifndef ASCII_TABLE_H
#define ASCII_TABLE_H

#include <util/memory.h>
#include <util/list.h>
#include <stdio.h>


#define String char const*

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER
} Alignment;

typedef struct line_list LineList;

typedef struct {
    LineList *lines;
} AsciiGraphicalTableBuilder;

void line(AsciiGraphicalTableBuilder *builder, Alignment alignment, char const *content, char const *comment);

void sep(AsciiGraphicalTableBuilder *builder, Alignment alignment, char const *content, char const *comment);

void close(AsciiGraphicalTableBuilder *builder, char const *content, char const *comment);

void printTable(AsciiGraphicalTableBuilder *builder, FILE *output, int indentation);

AsciiGraphicalTableBuilder *newAsciiGraphicalTableBuilder();

#endif