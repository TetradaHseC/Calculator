#include "operations.h"
#include <stdbool.h>
#include "structsAndEnums.h"

bool IsUnary(Operation operation) {
    switch (operation) {
        case EOpenParenthesis: // parenthesis are not unary but bot a binary too ;)
        case ECloseParenthesis:
        case EUnaryMinus:
        case ESin:
        case ECos:
        case ETg:
        case ELn:
        case ELog:
        case EAbs:
        case EExp:
        case EPhase:
        case ESqrt:
        case EReal:
        case EImag:
        case EMag:
            return true;
        default:
            return false;
    }
}
