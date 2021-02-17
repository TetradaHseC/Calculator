#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "parser.h"

#define DArray(type, name) typedef struct { type *array; int size, count;} name;
#define scase(var) if (strlen(#var) == i && strncmp(scaseval, #var, i) == 0)
#define elif else if
#define throw_error(message) fprintf(stderr, "Error: "#message); exit(-1)
#define string_pforeach(chrName, string) for (char *(chrName) = string; *(chrName) != '\0'; ++(chrName))

DArray(ComplexNumber, ComplexNumbersDArray)
DArray(Operation, OperationsDArray)

void AddToComplexNumbersDArray(ComplexNumbersDArray *dArray, ComplexNumber element);

void AddToOperationsDArray(OperationsDArray *dArray, Operation element);

void UnfoldComplexNumbersDArray(int *valc, ComplexNumber **valv, ComplexNumbersDArray *dArray);

void UnfoldOperationsDArray(int *valc, Operation **valv, OperationsDArray *dArray);

int IsComma(char chr);

int IsDigit(char chr);

int IsSymbol(char chr);

int IsAlpha(char chr);

int IsFunction(char *startStr);

ComplexNumber GetNumber(char **startStr);

ComplexNumber GetDefined(char **startStr);

Operation GetOperation(char **startStr);

Operation GetSymbolOperation(char chr);

void OnSymbol(OperationsDArray *pDOArray, const char *pc);

Operation GetLast(const OperationsDArray *dOArray);

bool IsESymbol(Operation operation);

void OnAlpha(ComplexNumbersDArray *dNArray, OperationsDArray *dOArray, char **pc);

void OnDigit(ComplexNumbersDArray *dNArray, char **pc);

void OnComma(OperationsDArray *dOArray);

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
    ComplexNumbersDArray dNArray = { NULL, 0, 0 };
    OperationsDArray dOArray = { NULL, 0, 0 };

    string_pforeach(pc, line) {
        if (IsComma(*pc)) {
            OnComma(&dOArray);
        } elif (IsDigit(*pc)) {
            OnDigit(&dNArray, &pc);
        } elif (IsAlpha(*pc)) {
            OnAlpha(&dNArray, &dOArray, &pc);
        } elif (IsSymbol(*pc)) {
            OnSymbol(&dOArray, pc);
        }
    }

    UnfoldComplexNumbersDArray(numc, numv, &dNArray);
    UnfoldOperationsDArray(opc, opv, &dOArray);
}

void OnComma(OperationsDArray *dOArray) {
    AddToOperationsDArray(dOArray, EComma);
}

void OnDigit(ComplexNumbersDArray *dNArray, char **pc) {
    AddToComplexNumbersDArray(dNArray, GetNumber(pc));
}

void OnAlpha(ComplexNumbersDArray *dNArray, OperationsDArray *dOArray, char **pc) {
    if (IsFunction(*pc)) {
        AddToOperationsDArray(dOArray, GetOperation(pc));
    } else {
        AddToComplexNumbersDArray(dNArray, GetDefined(pc));
    }
}

void OnSymbol(OperationsDArray *pDOArray, const char *pc) {
    Operation operation = GetSymbolOperation(*pc);

    AddToOperationsDArray(
            pDOArray,
            operation == EMinus &&
            (pDOArray->count == 0 || IsESymbol(GetLast(pDOArray))) ? EUnaryMinus : operation);
}

bool IsESymbol(Operation operation) {
    bool something = operation == EOpenParenthesis ||
                     operation == EPlus ||
                     operation == EMinus ||
                     operation == EMultiply ||
                     operation == EDivide;
    return something;
}

Operation GetLast(const OperationsDArray *dOArray) { return dOArray->array[dOArray->count - 1]; }

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

    *startStr += afterDotCount + 1 + (int)log10(number) - 1 + (isFake ? 1 : 0);

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

#define UnfoldDArray(type, name) \
void Unfold##name(int *valc, type **valv, name *dArray) { \
    *valc = (*dArray).count; \
    *valv = calloc(sizeof(type), *valc); \
    memcpy(*valv, (*dArray).array, sizeof(type) * *valc); \
    free((*dArray).array); \
}

UnfoldDArray(ComplexNumber, ComplexNumbersDArray)
UnfoldDArray(Operation, OperationsDArray)

int IsComma(char chr) {
    return chr == ',';
}
