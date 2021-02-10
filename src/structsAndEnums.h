#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct { double real, fake; char *definedName; } ComplexNumber;

typedef enum { EPlus, EMinus, EMultiply, EDivide, EOpenParenthesis, ECloseParenthesis, ETg, ELn, ECos, ESin, EPow, EAbs, EExp, EMag, ELog, EReal, EImag, ESqrt, EPhase } Operation;

#endif//STRUCTS_H
