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

bool IsUnaryMinus(OperationsDArray *previousOperations, Operation operation);

bool IsGeneratesUnaryMinus(const OperationsDArray *operations);

void OnComma(OperationsDArray *dOArray);

void OnDigit(ComplexNumbersDArray *dNArray, char **pc);

void OnAlpha(ComplexNumbersDArray *dNArray, OperationsDArray *dOArray, char **pc);

void OnSymbol(OperationsDArray *dOArray, const char *pc);

bool IsFunctionWithLength2(int len, const char *line);

bool IsFunctionWithLength3(int len, const char *line);

bool IsFunctionWithLength4(int len, const char *line);

bool IsFunctionWithLength5(int len, const char *line);

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

    for (char *pc = line; *pc != '\0'; ++pc) {
        if (*pc == ' ') {
        } elif (IsComma(*pc)) {
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

void OnSymbol(OperationsDArray *dOArray, const char *pc) {
    Operation operation = GetSymbolOperation(*pc);

    AddToOperationsDArray(dOArray, IsUnaryMinus(dOArray, operation) ? EUnaryMinus : operation);
}

void OnAlpha(ComplexNumbersDArray *dNArray, OperationsDArray *dOArray, char **pc) {
    if (IsFunction(*pc)) {
        AddToOperationsDArray(dOArray, GetOperation(pc));
    } else {
        AddToComplexNumbersDArray(dNArray, GetDefined(pc));
    }
}

void OnDigit(ComplexNumbersDArray *dNArray, char **pc) { AddToComplexNumbersDArray(dNArray, GetNumber(pc)); }

void OnComma(OperationsDArray *dOArray) {
    AddToOperationsDArray(dOArray, EComma);
}

bool IsUnaryMinus(OperationsDArray *previousOperations, Operation operation) {
    return (operation) == EMinus && IsGeneratesUnaryMinus(previousOperations);
}

bool IsGeneratesUnaryMinus(const OperationsDArray *operations) {
    return ((*operations).count == 0 ||
            (*operations).array[(*operations).count - 1] == EOpenParenthesis ||
            (*operations).array[(*operations).count - 1] == EPlus ||
            (*operations).array[(*operations).count - 1] == EMinus ||
            (*operations).array[(*operations).count - 1] == EMultiply ||
            (*operations).array[(*operations).count - 1] == EDivide);
}

int IsDigit(char chr) {
    return ((chr >= '0') && (chr <= '9'));
}

int IsAlpha(char chr) {
    return (((chr >= 'a') && (chr <= 'z')) || ((chr >= 'A') && (chr <= 'Z')));
}

int IsSymbol(char chr) {
    return (!IsAlpha(chr) && !IsDigit(chr) && (chr != ' '));
}

int IsFunction(char *startStr) {
    bool result;
    int len = 0;
    while (IsAlpha(*(startStr + len++)));
    char line[len];
    memset(line, 0, len);
    strncpy(line, startStr, --len);

    result = IsFunctionWithLength2(len, line) ||
             IsFunctionWithLength3(len, line) ||
             IsFunctionWithLength4(len, line) ||
             IsFunctionWithLength5(len, line);

    return result;
}

bool IsFunctionWithLength5(int len, const char *line) {
    return len == 5 && strcmp(line, "phase") == 0;
}

bool IsFunctionWithLength4(int len, const char *line) {
    return len == 4 && ((strcmp(line, "real") == 0) ||
                            (strcmp(line, "imag") == 0) ||
                            (strcmp(line, "sqrt") == 0));
}

bool IsFunctionWithLength3(int len, const char *line) {
    return len == 3 && ((strcmp(line, "cos") == 0) ||
                            (strcmp(line, "sin") == 0) ||
                            (strcmp(line, "pow") == 0) ||
                            (strcmp(line, "abs") == 0) ||
                            (strcmp(line, "exp") == 0) ||
                            (strcmp(line, "mag") == 0) ||
                            (strcmp(line, "log") == 0));
}

bool IsFunctionWithLength2(int len, const char *line) {
    return len == 2 && ((strcmp(line, "tg") == 0) || (strcmp(line, "ln") == 0));
}

ComplexNumber GetNumber(char **startStr) {
    ComplexNumber result = { 0, NULL };
    double number = 0;
    int afterDotCount = 0;
    bool isAfterDot = false;
    bool isFake = false;

    char *pc;
    for (pc = *startStr; IsDigit(*pc) || *pc == 'i' || *pc == '.'; ++pc) {
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
    result.number = number * (isFake ? I : 1);

    *startStr = pc - 1;
    if (**startStr == ' ') (*startStr)++;
    if (isFake) (*startStr)++;

    return result;
}

ComplexNumber GetDefined(char **startStr) {
    ComplexNumber result = { 0, NULL };
    int size = 0;

    for (;IsAlpha((*startStr)[size]) || IsDigit((*startStr)[size]); size++);

    if (size == 1 && (*startStr)[0] == 'i') {
        result.number = 1 * I;
    } else {
        result.definedName = calloc(size + 1, sizeof(char));
        strncpy(result.definedName, *startStr, size);
        *startStr += size - 1;
    }

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

    throw_error("it is not operation(GetOperation)");
}

Operation GetSymbolOperation(char chr) {
    switch (chr) {
        case '+': return EPlus;
        case '-': return EMinus;
        case '/': return EDivide;
        case '*': return EMultiply;
        case '(': return EOpenParenthesis;
        case ')': return ECloseParenthesis;
        case '^': return EPow;
        default: throw_error("it is not operation(GetSymbolOperation)");
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
