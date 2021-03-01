#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <printf.h>
#include "output.h"
#include "utils.h"

void Output(char *filename, ComplexNumber answer) {
    char result[200] = {0};
    double real = creal(answer.number),
           imag = cimag(answer.number);

    if (fabs(real - (int)real) < 0.01) {
        sprintf(result + strlen(result), "%d", (int) real);
    } else {
        sprintf(result + strlen(result), "%.2lf", real);
    }

    if (fabs(imag) < 0.01) {
    } else {
        sprintf(result + strlen(result), " + ");
        if (fabs(real - (int) real) < 0.01) {
            sprintf(result + strlen(result), "%di", (int) imag);
        } else {
            sprintf(result + strlen(result), "%.2fi", imag);
        }
    }

    FILE *file;
    char *temp = GetFullPath(filename);

    file = fopen(temp, "w");
    if (file == NULL) { // we receive from user not a relative path
        file = fopen(filename, "r");
    } if (file == NULL) {
        // somehow throw error
    }
    free(temp);

    fputs(result, file);

    fclose(file);
}
