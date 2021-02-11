#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "parser.h"

#define DArray(type, name) typedef struct { type *array; int size, count;} name;
#define scase(var) if (strlen(#var) == i && strncmp(scaseval, #var, i) == 0)
#define elif else if
#define throw_error(message) fprintf(stderr, "Error: "#message); 0 / 0;

DArray(ComplexNumber, ComplexNumbersDArray)
DArray(Operation, OperationsDArray)

void AddToComplexNumbersDArray(ComplexNumbersDArray *dArray, ComplexNumber element);

void AddToOperationsDArray(OperationsDArray *dArray, Operation element);

int IsDigit(char chr);

int IsSymbol(char chr);

int IsAlpha(char chr);

int IsFunction(char *startStr);

ComplexNumber GetNumber(char **startStr);

ComplexNumber GetDefined(char **startStr);

Operation GetOperation(char **startStr);

Operation GetSymbolOperation(char chr);

DictEntire ParseDefinition(char *line) {
    DictEntire result;

    char *p = line;
    while (*++p != '=' && *p != ' ');

    result.key = calloc(p - line + 1, sizeof(char));
    strncpy(result.key, line, p - line);
    result.key[p - line] = '\0';

    while (*++p == '=' || *p == ' ');

    ParseExpression(p, &result.value.numc, &result.value.numv, &result.value.opc, &result.value.opv);

    return result;
}

void ParseExpression(char *line, int *numc, ComplexNumber **numv, int *opc, Operation **opv) {
#define NComplexNumberContext(block) { ComplexNumber cn = { 0.0, 0.0 }; block; }
    ComplexNumbersDArray dNArray = { NULL, 0, 0 };
    OperationsDArray dOArray = { NULL, 0, 0 };

    for (char *pc = line; *pc != '\0'; ++pc) {
        if (IsDigit(*pc)) {
            NComplexNumberContext({
                                      AddToComplexNumbersDArray(&dNArray, GetNumber(&pc));
                                  })
        } elif (IsAlpha(*pc)) {
            if (IsFunction(pc)) {
                AddToOperationsDArray(&dOArray, GetOperation(&pc));
            } else {
                NComplexNumberContext({
                                          AddToComplexNumbersDArray(&dNArray, GetDefined(&pc));
                                      })
            }
        } elif (IsSymbol(*pc)) {
            AddToOperationsDArray(&dOArray, GetSymbolOperation(*pc));
        }
    }

    *numc = dNArray.count;
    *numv = calloc(sizeof(ComplexNumber), *numc);
    memcpy(*numv, dNArray.array, sizeof(ComplexNumber) * *numc);
    free(dNArray.array);

    *opc = dOArray.count;
    *opv = calloc(sizeof(Operation), *opc);
    memcpy(*opv, dOArray.array, sizeof(Operation) * *opc);
    free(dOArray.array);
}

int IsDigit(char chr) {
    return ((chr >= '0')&&(chr <= '9'));
}

int IsAlpha(char chr) {
    return (((chr >= 'a')&&(chr <= 'z'))||((chr >= 'A')&&(chr <= 'Z')));
}

int IsSymbol(char chr) {
    return ((!IsAlpha(chr))&&(!IsDigit(chr))&&(chr != ' '));
}

int IsFunction(char *startStr) {
    int len = 0;
    while (IsAlpha(*(startStr + len++)));
    len--;
    char *line = calloc(len, sizeof(char));
    strncpy(line, startStr, len);

    if (len == 2) {
        if ((strcmp(line, "tg") == 0) ||
            (strcmp(line, "ln") == 0)) {
            return 1;
        } else {
            return 0;
        }
    } elif (len == 3) { // TODO: ADD file for operations names
        if ((strcmp(line, "cos") == 0) ||
            (strcmp(line, "sin") == 0) ||
            (strcmp(line, "pow") == 0) ||
            (strcmp(line, "abs") == 0) ||
            (strcmp(line, "exp") == 0) ||
            (strcmp(line, "mag") == 0) ||
            (strcmp(line, "log") == 0)
        ) {
            return 1;
        } else {
            return 0;
        }
    } elif (len == 4) {
        if ((strcmp(line, "real") == 0) ||
            (strcmp(line, "imag") == 0) ||
            (strcmp(line, "sqrt") == 0)
        ) {
            return 1;
        } else {
            return 0;
        }
    } elif (len == 5) {
        if (strcmp(line, "phase") == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

ComplexNumber GetNumber(char **startStr) {
    ComplexNumber result = { 0, 0, NULL };
    double number = 0;
    int afterDotCount = 0;
    bool isAfterDot = false;
    bool isFake = false;

    for (char *pc = *startStr; *pc != '\0' && *pc != ' '; ++pc) {
        if (IsAlpha(*pc)) {
            isFake = *pc == 'i';
            break;
        } elif (IsDigit(*pc)) {
            number *= 10;
            number += *pc - '0';

            if (isAfterDot)
                afterDotCount++;
        } elif (*pc == '.') {
            isAfterDot = true;
        }
    }

    number /= pow(10, afterDotCount);
    if (!isFake) {
        result.real = number;
    } else {
        result.fake = number;
    }

    *startStr += afterDotCount + 1 + (int)log10(number) - 1;

    return result;
}

ComplexNumber GetDefined(char **startStr) {
    ComplexNumber result = { 0, 0, NULL };
    int size = 0;

    int i = 0;
    for (;;i++) {
        if (IsAlpha((*startStr)[i])) {
            size++;
        } else {
            break;
        }
    }

    result.definedName = calloc(i + 1, sizeof(char));
    strncpy(result.definedName, *startStr, i);
    *startStr += i - 1;

    return result;
}

Operation GetOperation(char **startStr) {
    int i = 0;
    while (IsAlpha((*startStr)[i++]));
    i--;

    char strval[i];

    strncpy(strval, *startStr, i);
    *startStr += i - 1;

    if (i == 1) return GetSymbolOperation(**startStr);

#define scaseval strval
    scase(tg) return ETg;
    scase(ln) return ELn;
    scase(cos) return ECos;
    scase(sin) return ESin;
    scase(pow) return EPow;
    scase(abs) return EAbs;
    scase(exp) return EExp;
    scase(mag) return EMag;
    scase(log) return ELog;
    scase(real) return EReal;
    scase(imag) return EImag;
    scase(sqrt) return ESqrt;
    scase(phase) return EPhase;

    throw_error("it is not operation");
}

Operation GetSymbolOperation(char chr) {
    switch (chr) {
        case '+': return EPlus;
        case '-': return EMinus;
        case '/': return EDivide;
        case '*': return EMultiply;
        case '(': return EOpenParenthesis;
        case ')': return ECloseParenthesis;
        default: throw_error("it is not operation");
    }
}

#define AddToDArray(type, name) \
void AddTo##name(name *dArray, type element) { \
    if (dArray->size == dArray->count) {                                              \
        type *newArray = calloc(dArray->size * 2 + 1, sizeof(type));\
        for (int i = 0; i < dArray->count; ++i) {                                     \
            newArray[i] = dArray->array[i];                                           \
        }                       \
        free(dArray->array);    \
        dArray->array = newArray;              \
        dArray->size = dArray->size * 2 + 1;\
    }                           \
                                \
    dArray->array[dArray->count++] = element;                                         \
}

AddToDArray(ComplexNumber, ComplexNumbersDArray)
AddToDArray(Operation, OperationsDArray)
