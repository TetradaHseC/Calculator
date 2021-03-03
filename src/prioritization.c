#include <string.h>

int CheckThePriority(char operation[5]){
    if(!strcmp(operation, "sqrt"))
        operation[0] = ':';
    switch(* operation){
        case '+': case'-': return 0;
        case '*': case'/': return 1;
        case '^': case ':': return 2;
        case '(': case ')': return 3;
        default:  return 4;
    }
}