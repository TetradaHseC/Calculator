#include <stdlib.h>
#include <stdbool.h>
#include "src/parser.h"
#include "src/input.h"
#include "src/ui.h"
#include "src/evaluator.h"
#include "src/output.h"
#include "src/parenthases_parser.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main() {
    char *expressionString;
    char **definitionsStrings;
    int definitionsCount;

    ComplexNumber answer = { 0, 0 };
    bool isCalculated = false;

    // loop will stop in DoUIIteration
    while (true) {
        if (isCalculated) {
            char result[200] = "Ответ: ";
            BeautifyOutput(result, &answer);
            DoUIIteration(result);
        } else {
            DoUIIteration(NULL);
        }

        GetExpressionAndDefines(inputFile, &expressionString, &definitionsStrings, &definitionsCount);
        EjectParentheses(&expressionString, &definitionsStrings, &definitionsCount);

        ParsedExpression expression;
        ParseExpression(expressionString,
                        &expression.numc,
                        &expression.numv,
                        &expression.opc,
                        &expression.opv);
        free(expressionString);

        DictEntire dictEntires[definitionsCount];
        for (int i = 0; i < definitionsCount; ++i) {
            dictEntires[i] = ParseDefinition(definitionsStrings[i]);
            free(definitionsStrings[i]);
        }
        free(definitionsStrings);

        answer = Evaluate(expression, definitionsCount, dictEntires);
        isCalculated = true;
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
}
#pragma clang diagnostic pop
