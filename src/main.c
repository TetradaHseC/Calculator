#include "parser.h"
#include "input.h"

void SomeFuncForEvaluating(ParsedExpression expression, int count, DictEntire **entires);

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

    //printf("%s", CNumberToString(SomeFuncForEvaluating(expression, entiresCount, dictEntires)));

    return 0;
}
