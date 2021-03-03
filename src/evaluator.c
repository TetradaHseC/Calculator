#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "evaluator.h"
#include "complex.h"
#include "math.h"
#include "operations.h"
#include "stdlib.h"
#include "structsAndEnums.h"

#define PI_STRING "PI"
#define E_STRING "e"

int GetPrior(Operation operation){
    switch(operation) {
        case ECloseParenthesis:
            return 0;
        case EPlus:
        case EMinus:
            return 1;
        case EMultiply:
        case EDivide:
            return 2;
        default:
            return 3;
    }

}

void EvaluateOperation(Operation operation, ComplexNumber* numbersarray, int numb_id);

void EvaluateBinaryOperation(Operation operation, ComplexNumber* numbersarray, int numb_id);

void EvaluateUnaryOperation(Operation operation, ComplexNumber* numbersarray, int numb_id);

DictEntire *GetEntire(DictEntire *entires, int entiresCount, char *name);

ComplexNumber ExecDefined(ParsedExpression *expression, int count, DictEntire *entires, int id_numb);

bool IsConst(char *name);

ComplexNumber GetConst(char *name);

ComplexNumber Evaluate(ParsedExpression expression, int count, DictEntire *entires) {
    int id_oper = 0; // operations count from args
    int id_numb = 0; // operations count from args
    int operCount = 0; // local operations count
    int numbCount = 0; // local numbers count

    Operation curroperation[expression.opc];
    ComplexNumber currnumbers[expression.numc];

    currnumbers[numbCount++] = ExecDefined(&expression, count, entires, id_numb);
    id_numb++;

    while (operCount >= 0 && id_oper <= expression.opc) {
        while ((operCount == 0 || curroperation[operCount - 1] == EOpenParenthesis ||
                (GetPrior(curroperation[operCount - 1]) <= GetPrior(expression.opv[id_oper]))) &&
               (id_oper < expression.opc)) {
            curroperation[operCount++] = expression.opv[id_oper++];

            if (!IsUnary(curroperation[operCount - 1])) {
                currnumbers[numbCount++] = ExecDefined(&expression, count, entires, id_numb);
                id_numb++;
            }
        }
        if (expression.opv[id_oper] == ECloseParenthesis)
            curroperation[operCount++] = expression.opv[id_oper++];

        while (operCount > 0) {
            if (curroperation[operCount - 1] == EOpenParenthesis) {
                operCount--;
                break;
            }
            if (curroperation[operCount - 1] == ECloseParenthesis) {
                operCount--;
                continue;
            }

            EvaluateOperation(curroperation[--operCount], currnumbers, --numbCount);

            if (IsUnary(curroperation[operCount])) numbCount++;

            if (numbCount == 0) numbCount = 1;
        }

        if (id_oper == expression.opc && operCount == 0)
            break;
    }
    return currnumbers[0];
}

ComplexNumber ExecDefined(ParsedExpression *expression, int count, DictEntire *entires, int id_numb) {
    if ((*expression).numv[id_numb].definedName == NULL)
        return (*expression).numv[id_numb];

    if (IsConst((*expression).numv[id_numb].definedName))
        return GetConst((*expression).numv[id_numb].definedName);

    return Evaluate(GetEntire(entires, count, (*expression).numv[id_numb].definedName)->value, count, entires);
}

ComplexNumber GetConst(char *name) {
    ComplexNumber result = { 0, 0 };

    if (strcmp(name, PI_STRING) == 0)
        result.number = M_PI;
    if (strcmp(name, E_STRING) == 0)
        result.number = M_E;

    return result;
}

bool IsConst(char *name) {
    return strcmp(name, PI_STRING) == 0 || strcmp(name, E_STRING) == 0;
}

DictEntire *GetEntire(DictEntire *entires, int entiresCount, char *name) {
    for(int i=0;i<entiresCount;++i) {
        if (!strcmp(entires[i].key,name))
            return entires + i;
    }
    exit(404);// page not found
}

void EvaluateOperation(Operation operation,ComplexNumber* numbersarray,int numb_id) {
    if (IsUnary(operation)) {
        EvaluateUnaryOperation(operation, numbersarray, numb_id);
    } else {
        EvaluateBinaryOperation(operation, numbersarray, numb_id);
    }
}

void EvaluateBinaryOperation(Operation operation, ComplexNumber* numbersarray,int numb_id){
    switch(operation) {
        case EMinus:
            numbersarray[numb_id - 1].number = numbersarray[numb_id-1].number - numbersarray[numb_id].number;
            break;
        case EPlus:
            numbersarray[numb_id - 1].number = numbersarray[numb_id].number + numbersarray[numb_id - 1].number;
            break;
        case EMultiply:
            numbersarray[numb_id - 1].number = numbersarray[numb_id].number * numbersarray[numb_id - 1].number;
            break;
        case EDivide:
            numbersarray[numb_id - 1].number = numbersarray[numb_id - 1].number / numbersarray[numb_id].number;
            break;
        case EPow:
            numbersarray[numb_id - 1].number = cpow(numbersarray[numb_id-1].number, numbersarray[numb_id].number);
            break;
        default:
            fprintf(stderr, "Wrong Operation operation in EvaluateBinaryOperation");
    }
}

void EvaluateUnaryOperation(Operation operation, ComplexNumber* numbersarray, int numb_id) {
    switch (operation) {
        case ETg:
            numbersarray[numb_id].number = ctan(numbersarray[numb_id].number);
            break;
        case ELn:
            numbersarray[numb_id].number = clog(numbersarray[numb_id].number);
            break;
        case EUnaryMinus:
            numbersarray[numb_id].number = 0 - numbersarray[numb_id].number;
            break;
        case ELog:
            numbersarray[numb_id].number = clog(numbersarray[numb_id].number) / clog(10);
            break;
        case ESqrt:
            numbersarray[numb_id].number = csqrt(numbersarray[numb_id].number);
            break;
        case ECos:
            numbersarray[numb_id].number = ccos(numbersarray[numb_id].number);
            break;
        case ESin:
            numbersarray[numb_id].number = csin(numbersarray[numb_id].number);
            break;
        case EAbs:
            numbersarray[numb_id].number = cabs(numbersarray[numb_id].number);
            break;
        case EExp:
            numbersarray[numb_id].number = cexp(numbersarray[numb_id].number);
            break;
        case EReal:
            numbersarray[numb_id].number = creal(numbersarray[numb_id].number);
            break;
        case EImag:
            numbersarray[numb_id].number = cimag(numbersarray[numb_id].number);
            break;
        case EMag:
            numbersarray[numb_id].number = cabs(numbersarray[numb_id].number);
            break;
        case EPhase:
            numbersarray[numb_id].number = carg(numbersarray[numb_id].number);
            break;
        default:
            fprintf(stderr, "Wrong Operation operation in EvaluateUnaryOperation");
    }
}