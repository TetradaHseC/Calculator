#include <stdlib.h>
#include "parser.h"

void SomeFuncForEvaluating(ParsedExpression expression, int count, DictEntire **entires);

int main() {
    DictEntire dictEntire1 = ParseDefinition("a = 12 + b - c - 3 * 4 / (5 + 6i + sin(7 - (8 + 9 - 10)))");
    DictEntire dictEntire2 = ParseDefinition("b = 12 - c");
    DictEntire dictEntire3 = ParseDefinition("c = 4*pi");
    int entiresCount = 3;

    DictEntire **dictEntires = calloc(entiresCount, sizeof(DictEntire *));
    dictEntires[0] = &dictEntire1;
    dictEntires[1] = &dictEntire2;
    dictEntires[2] = &dictEntire3;

    ParsedExpression expression;
    ParseExpression("a + b + c",
                    &expression.numc,
                    &expression.numv,
                    &expression.opc,
                    &expression.opv);

    //printf("%s", CNumberToString(SomeFuncForEvaluating(expression, entiresCount, dictEntires)));

    return 0;
}
