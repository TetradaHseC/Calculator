#include "evaluator.h"
#include "evaluator.h"
#include <stdbool.h>
#include "evaluator.h"
#include "complex.h"
#include "math.h"
#include "operations.h"
#include "stdlib.h"
#include "structsAndEnums.h"
#include<string.h>

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

DictEntire *MoveToEndEntire(DictEntire *entires, int count, char *name);

ComplexNumber Evaluate(ParsedExpression expression, int count, DictEntire *entires) {
    int id_oper = 0; // operations count from args
    int id_numb = 0; // operations count from args
    int operCount = 0; // local operations count
    int numbCount = 0; // local numbers count

    Operation curroperation[expression.opc];
    ComplexNumber currnumbers[expression.numc];

    currnumbers[numbCount++] = (expression.numv[id_numb].definedName != NULL) ?
                               Evaluate(GetEntire(entires, count, expression.numv[id_numb].definedName)->value, count, entires) :
                               expression.numv[id_numb];
    id_numb++;
    //curroperation[operCount++] = expression.opv[id_oper++];
    while (operCount >= 0 && id_oper <= expression.opc) {
        while ((operCount == 0 || curroperation[operCount - 1] == EOpenParenthesis ||
                (GetPrior(curroperation[operCount - 1]) <= GetPrior(expression.opv[id_oper]))) &&
               (id_oper < expression.opc)) {
            curroperation[operCount++] = expression.opv[id_oper++];

            if (!IsUnary(curroperation[operCount - 1])) {
                currnumbers[numbCount++] = (expression.numv[id_numb].definedName != NULL) ?
                                           Evaluate(GetEntire(entires, count, expression.numv[id_numb].definedName)->value, count, entires) :
                                       expression.numv[id_numb];
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

DictEntire *MoveToEndEntire(DictEntire *entires, int count, char *name) {
    DictEntire *ptemp = GetEntire(entires, count, name);
    DictEntire temp = *ptemp;

    *ptemp = entires[count - 1];
    entires[count - 1] = temp;

    return entires;
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
    switch(operation){
        case EMinus:
            numbersarray[numb_id - 1].number = numbersarray[numb_id].number - numbersarray[numb_id - 1].number;
            break;
        case EPlus:
            numbersarray[numb_id - 1].number = numbersarray[numb_id].number + numbersarray[numb_id - 1].number;
            break;
        case EMultiply:
            numbersarray[numb_id - 1].number = numbersarray[numb_id].number * numbersarray[numb_id - 1].number;
            break;
        case EDivide:
            numbersarray[numb_id - 1].number = numbersarray[numb_id].number / numbersarray[numb_id - 1].number;
            break;
        case EPow:
            numbersarray[numb_id - 1].number = cpow(numbersarray[numb_id].number,numbersarray[numb_id - 1].number);
            break;
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
    }
}