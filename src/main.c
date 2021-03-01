#include <stdlib.h>
#include <printf.h>
#include "parser.h"
#include "input.h"
#include "ui.h"
#include "evaluator.h"

int main() {
    char *expressionString;
    char **definitionsStrings;
    int definitionsCount;

    GetExpressionAndDefines("../res/calc_input.txt", &expressionString, &definitionsStrings, &definitionsCount);

    ParsedExpression expression;
    ParseExpression(expressionString,
                    &expression.numc,
                    &expression.numv,
                    &expression.opc,
                    &expression.opv);

    DictEntire dictEntires[definitionsCount];
    for (int i = 0; i < definitionsCount; ++i) {
        dictEntires[i] = ParseDefinition(definitionsStrings[i]);
    }

    //DrawUI();

    ComplexNumber answer = Evaluate(expression, definitionsCount, dictEntires);
    printf("%.2lf + %.2lfi", creal(answer.number), cimag(answer.number));

    free(expression.opv);
    for (int j = 0; j < expression.numc; ++j) {
        if (expression.numv[j].definedName)
            free(expression.numv[j].definedName);
    }
    free(expression.numv);

    for (int i = 0; i < definitionsCount; ++i) {
        free(dictEntires[i].key);

        ParsedExpression defined = dictEntires[i].value;
        free(defined.opv);
        for (int j = 0; j < defined.numc; ++j) {
            if (defined.numv[j].definedName)
                free(defined.numv[j].definedName);
        }
        free(defined.numv);
    }

    return 0;
}
