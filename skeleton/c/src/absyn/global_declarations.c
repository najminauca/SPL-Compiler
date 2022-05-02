/*
 * global_declarations.c -- abstract syntax for global declarations
 */

#include "global_declarations.h"
#include "lists.h"
#include "parameter_declaration.h"
#include "variable_declaration.h"
#include "statements.h"
#include "type_expressions.h"
#include <util/memory.h>
#include <absyn/printing/printing_helpers.h>

static GlobalDeclaration *newGlobalDeclaration(Position position, global_declaration_kind kind, Identifier *name) {
    GlobalDeclaration *node = new(GlobalDeclaration);

    node->position = position;
    node->kind = kind;
    node->name = name;
    return node;
}

GlobalDeclaration *newTypeDeclaration(Position position, Identifier *name, TypeExpression *ty) {
    GlobalDeclaration *node = newGlobalDeclaration(position, DECLARATION_TYPEDECLARATION, name);
    node->u.typeDeclaration.typeExpression = ty;
    return node;
}

GlobalDeclaration *newProcedureDeclaration(Position position, Identifier *name, ParameterDeclarationList *params, VariableDeclarationList *decls, StatementList *body) {
    GlobalDeclaration *node = newGlobalDeclaration(position, DECLARATION_PROCEDUREDECLARATION, name);
    node->u.procedureDeclaration.parameters = params;
    node->u.procedureDeclaration.variables = decls;
    node->u.procedureDeclaration.body = body;
    return node;
}

static void printProcedureDeclaration(FILE *out, int indentation, GlobalDeclaration *self) {
    fprintf(out, "ProcedureDeclaration(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, self->name);
    fprintf(out, ",\n");

    // Print Parameter List
    {
        indent(out, indentation + INDENTATION_INCREMENT);
        fprintf(out, "Parameters(");
        printParameterDeclarationList(out, indentation + 2 * INDENTATION_INCREMENT, true, self->u.procedureDeclaration.parameters);
        fprintf(out, "),\n");
    }

    // Print Variable list
    {
        indent(out, indentation + INDENTATION_INCREMENT);
        fprintf(out, "Variables(");
        printVariableDeclarationList(out, indentation + 2 * INDENTATION_INCREMENT, true, self->u.procedureDeclaration.variables);
        fprintf(out, "),\n");
    }

    // Print Body
    {
        indent(out, indentation + INDENTATION_INCREMENT);
        fprintf(out, "Body(");
        printStatementList(out, indentation + 2 * INDENTATION_INCREMENT, true, self->u.procedureDeclaration.body);
        fprintf(out, ")");
    }

    // End of ProcedureDeclaration
    fprintf(out, ")");
}

static void printTypeDeclaration(FILE *out, int indentation, GlobalDeclaration *globalDeclaration) {
    fprintf(out, "TypeDeclaration(\n");
    printIdentifier(out, indentation + INDENTATION_INCREMENT, globalDeclaration->name);
    fprintf(out, ",\n");
    printTypeExpression(out, indentation + INDENTATION_INCREMENT, globalDeclaration->u.typeDeclaration.typeExpression);
    fprintf(out, ")");
}

void printGlobalDeclaration(FILE *out, int indentation, GlobalDeclaration *self) {
    indent(out, indentation);

    if (self == NULL) {
        fprintf(out, "NULL");
        return;
    }

    switch (self->kind) {
        case DECLARATION_PROCEDUREDECLARATION:
            printProcedureDeclaration(out, indentation, self);
            break;
        case DECLARATION_TYPEDECLARATION:
            printTypeDeclaration(out, indentation, self);
            break;
        default:
            printUnknownKind(out, self->kind);
    }
}
