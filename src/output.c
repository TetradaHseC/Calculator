#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "output.h"
#include "utils.h"

void Output(char *filename, ComplexNumber answer) {
    FILE *file;
    char *temp = GetFullPath(filename);

    file = fopen(temp, "w");
    if (file == NULL) { // we receive from user not a relative path
        file = fopen(filename, "w");
    } if (file == NULL) {
        // somehow throw error
    }
    free(temp);

    char result[200] = {0};
    BeautifyOutput(result, &answer);
    fprintf(file, "%s", result);

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

    if (fabs(imag) < 0.005) {
    } else {
        sprintf(dest + strlen(dest), (imag >= 0) ? " + " : " - ");
        if (fabs(imag - (int) imag) < 0.01) {
            sprintf(dest + strlen(dest), "%di", abs((int) imag));
        } else {
            sprintf(dest + strlen(dest), "%.2fi", fabs(imag));
        }
    }
}
