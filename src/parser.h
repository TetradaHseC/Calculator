#ifndef PARSER_H
#define PARSER_H
#include "structsAndEnums.h"

DictEntire ParseDefinition(char *line);

void ParseExpression(char *line, int *numc, ComplexNumber **numv, int *opc, Operation **opv);

#endif//PARSER_H
