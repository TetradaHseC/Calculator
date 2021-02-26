#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "input.h"

char *stolast(char *string);

void GetExpressionAndDefines(char *filename, char **expressionString, char ***definitionLines, int *definitionCount) {
    FILE *file = fopen(filename, "r");

    *expressionString = calloc(200, sizeof(char));
    fgets(*expressionString, 200, file);
    if (*stolast(*expressionString) == '\n')
        *stolast(*expressionString) = '\0';
    else
        *(stolast(*expressionString) + 1) = '\0';

    *definitionLines = calloc(200, sizeof(char *));
    for (int i = 0; i < 200; ++i) {
        (*definitionLines)[i] = calloc(200, sizeof(char));
    }

    while (!feof(file)) {
        fgets((*definitionLines)[*definitionCount], 200, file);
        if (*stolast((*definitionLines)[*definitionCount]) == '\n')
            *stolast((*definitionLines)[*definitionCount]) = '\0';
        (*definitionCount)++;
    }
}

char *stolast(char *string) {
    return string + strlen(string) - 1;
}
