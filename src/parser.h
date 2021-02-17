#ifndef PARSER_H
#define PARSER_H
#include "structsAndEnums.h"

/// DictEntire.value.numv[n].definedName must be freed after usage if it not NULL
DictEntire ParseDefinition(char *line);

/// numv[n].definedName must be freed after usage if it not NULL
void ParseExpression(char *line, int *numc, ComplexNumber **numv, int *opc, Operation **opv);

#endif//PARSER_H
