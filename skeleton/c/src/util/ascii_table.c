#include "ascii_table.h"

#include "string_ops.h"
#include <util/list.h>

#include <string.h>

#define VERTICAL_SEP  "|"
#define HORIZONTAL_SEP  "─"
#define LEFT_CROSS "├"
#define RIGHT_CROSS  "┤"
#define BOTTOM_LEFT_CORNER  "└"
#define BOTTOM_RIGHT_CORNER  "┘"

static int max(int a, int b) {
    return a >= b ? a : b;
}

typedef struct line {
    String left;
    String right;
    String padding;
    String content;
    Alignment alignment;
    String comment;
} Line;

DECLARE_LIST_STRUCT(line_list, LineList, Line)

DEFINE_EMPTY_LIST(newLineList, LineList)

DEFINE_LIST_APPEND(appendToLineList, LineList, Line, newLineList())

static void align(Alignment alignment, int width, const char *str, FILE *output, const char *padding) {
    switch (alignment) {
        case ALIGN_LEFT:
            fprintf(output, "%s", str);
            repeat(output, width - (int) strlen(str), padding);
            break;
        case ALIGN_CENTER: {
            int padding_count = width - (int) strlen(str);

            int pre = padding_count / 2;
            int post = padding_count - pre;

            repeat(output, pre, padding);
            fprintf(output, "%s", str);
            repeat(output, post, padding);
        }
            break;
        default:
            break;
    }
}

Line *newLine(const char *left, const char *right, const char *padding, const char *content, Alignment alignment, const char *comment) {
    Line *self = new(Line);

    self->left = left;
    self->right = right;
    self->padding = padding;
    self->content = content;
    self->alignment = alignment;
    self->comment = comment;

    return self;
}

void line(AsciiGraphicalTableBuilder *builder, Alignment alignment, const char *content, const char *comment) {
    appendToLineList(builder->lines, newLine(VERTICAL_SEP, VERTICAL_SEP, " ", content, alignment, comment));
}

void sep(AsciiGraphicalTableBuilder *builder, Alignment alignment, const char *content, const char *comment) {
    appendToLineList(builder->lines, newLine(LEFT_CROSS, RIGHT_CROSS, HORIZONTAL_SEP, content, alignment, comment));
}

void close(AsciiGraphicalTableBuilder *builder, const char *content, const char *comment) {
    appendToLineList(builder->lines, newLine(BOTTOM_LEFT_CORNER, BOTTOM_RIGHT_CORNER, HORIZONTAL_SEP, content, ALIGN_CENTER, comment));
}

void printLine(Line *line, FILE *output, int minWidth) {
    fprintf(output, "%s", line->left);
    fprintf(output, "%s", line->padding);
    align(line->alignment, minWidth, line->content, output, line->padding);
    fprintf(output, "%s", line->padding);
    fprintf(output, "%s %s\n", line->right, line->comment);
}

void printTable(AsciiGraphicalTableBuilder *builder, FILE *output, int indentation) {
    int minWidth = 0;
    {
        LineList *list = builder->lines;
        while (!list->isEmpty) {
            minWidth = max(minWidth, (int) strlen(list->head->content));
            list = list->tail;
        }
    }

    {
        LineList *list = builder->lines;
        while (!list->isEmpty) {
            repeat(output, indentation, " ");
            printLine(list->head, output, minWidth);
            list = list->tail;
        }
    }
}

AsciiGraphicalTableBuilder *newAsciiGraphicalTableBuilder() {
    AsciiGraphicalTableBuilder *self = new(AsciiGraphicalTableBuilder);
    self->lines = newLineList();

    return self;
}
