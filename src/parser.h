#ifndef PARSER_H
#define PARSER_H
#include "structsAndEnums.h"

void ParseExpression(char *line, int *numc, ComplexNumber **numv, int *opc, Operation **opv);

#endif//PARSER_H
