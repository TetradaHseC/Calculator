#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "parenthases_parser.h"

#define ALPHABET_CAPACITY 63 // 0-9 + a-z + A-Z + _ = 10 + 26 + 26 + 1 = 63

int GlobalParenthesesCount = 0;

void EjectParenthesesFromExpression(char *expressionString, char ***definitionLines, int *definitionCount);

void GenerateNewDefinition(char *start, char *end, char ***definitionLines, int *definitionCount);

void EjectParentheses(char **expressionString, char ***definitionLines, int *definitionCount) {
    int originalDefinitionCount = *definitionCount;
    for (int i = 0; i < originalDefinitionCount; ++i) {
        EjectParenthesesFromExpression((*definitionLines)[i], definitionLines, definitionCount);
    }
    EjectParenthesesFromExpression(*expressionString, definitionLines, definitionCount);
}

char GenerateName(int number);

void EjectParenthesesFromExpression(char *expressionString, char ***definitionLines, int *definitionCount) {
    int parenthesesCount = 0;
    int openParenthesesPositions[ALPHABET_CAPACITY];

    for (int i = 0; i < strlen(expressionString); ++i) {
        if (expressionString[i] == '(') {
            openParenthesesPositions[parenthesesCount++] = i;
        } else if (expressionString[i] == ')') {
            GenerateNewDefinition(
                    expressionString + openParenthesesPositions[--parenthesesCount],
                    expressionString + i,
                    definitionLines,
                    definitionCount);
            memset(expressionString + openParenthesesPositions[parenthesesCount],
                   ' ', (i - openParenthesesPositions[parenthesesCount] + 1) * sizeof(char));
            (expressionString + openParenthesesPositions[parenthesesCount])[1] = '_';
            (expressionString + openParenthesesPositions[parenthesesCount])[2] = GenerateName(GlobalParenthesesCount++);
        }
    }
}

void GenerateNewDefinition(char *start, char *end, char ***definitionLines, int *definitionCount) {
    char *newString = calloc(end - start + 3 + 1, sizeof(char));
    newString[0] = '_';
    newString[1] = GenerateName(GlobalParenthesesCount);
    newString[2] = '=';
    strncpy(newString+3, start+1, end - start);
    (*definitionLines)[(*definitionCount)++] = newString;
}

// 0-9, a-z, A-Z capacity = 62
char GenerateName(int number) {
    if(number<=9&&number>=0)
        return (char)(number+'0');
    else if(number>9&&number<=35)
        return (char)((number-10)+'a');
    else if(number>35&&number<=61)
        return (char)((number-36)+'A');
    else if(number==62)
        return '_';

    fprintf(stderr, "parentheses count are limited for your account please donate for more");
    exit(number);
}
