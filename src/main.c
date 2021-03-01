#include <stdlib.h>
#include <printf.h>
#include <stdbool.h>
#include "parser.h"
#include "input.h"
#include "ui.h"
#include "evaluator.h"
#include "output.h"

int main() {
    char *expressionString;
    char **definitionsStrings;
    int definitionsCount;

    while (true) {
        DrawUI();

        GetExpressionAndDefines(inputFile, &expressionString, &definitionsStrings, &definitionsCount);

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

        ComplexNumber answer = Evaluate(expression, definitionsCount, dictEntires);
        //printf("%.2lf + %.2lfi\n", creal(answer.number), cimag(answer.number));
        Output(outputFile, answer);

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
    }

    return 0;
}
