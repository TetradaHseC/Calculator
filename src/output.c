#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <printf.h>
#include "output.h"
#include "utils.h"

void Output(char *filename, ComplexNumber answer) {
    FILE *file;
    char *temp = GetFullPath(filename);

    file = fopen(temp, "w");
    if (file == NULL) { // we receive from user not a relative path
        file = fopen(filename, "r");
    } if (file == NULL) {
        // somehow throw error
    }
    free(temp);

    char result[200];
    BeautifyOutput(result, &answer);
    fputs(result, file);

    fclose(file);
}

void BeautifyOutput(char *dest, ComplexNumber *answer) {
    double real = creal((*answer).number),
           imag = cimag((*answer).number);

    if (fabs(real - (int)real) < 0.01) {
        sprintf(dest + strlen(dest), "%d", (int) real);
    } else {
        sprintf(dest + strlen(dest), "%.2lf", real);
    }

    if (fabs(imag) < 0.01) {
    } else {
        sprintf(dest + strlen(dest), " + ");
        if (fabs(imag - (int) imag) < 0.01) {
            sprintf(dest + strlen(dest), "%di", (int) imag);
        } else {
            sprintf(dest + strlen(dest), "%.2fi", imag);
        }
    }
}
