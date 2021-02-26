#ifndef STRUCTS_H
#define STRUCTS_H

#include <complex.h>

typedef enum { EComma, EPlus, EMinus, EUnaryMinus, EMultiply, EDivide, EOpenParenthesis, ECloseParenthesis, ETg, ELn, ECos, ESin, EPow, EAbs, EExp, EMag, ELog, EReal, EImag, ESqrt, EPhase } Operation;

typedef struct {
    double complex number;
    char *definedName;
} ComplexNumber;

typedef struct {
    int numc;
    ComplexNumber *numv;
    int opc;
    Operation *opv;
} ParsedExpression;

typedef struct {
    char *key;
    ParsedExpression value;
} DictEntire;

#endif//STRUCTS_H
