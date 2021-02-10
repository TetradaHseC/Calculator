#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum { EPlus, EMinus, EMultiply, EDivide, EOpenParenthesis, ECloseParenthesis, ETg, ELn, ECos, ESin, EPow, EAbs, EExp, EMag, ELog, EReal, EImag, ESqrt, EPhase } Operation;

typedef struct {
    double real, fake;
    char *definedName;
} ComplexNumber;

typedef struct {
    int numc;
    ComplexNumber *numv;
    int opc;
    Operation *opv;
} ParsedExpression;

#endif//STRUCTS_H
