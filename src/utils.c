#include <zconf.h>
#include <stdlib.h>
#include <memory.h>
#include "utils.h"

void RemoveLastDir(char *path);

char *GetFullPath(char *relativePath) {
    char *fullPath = getcwd(calloc(300, sizeof(char)), 300 * sizeof(char));

    for (char *p = relativePath; *p != 0; ++p) {
        if (strlen(p) >= 2 && (p[0] == '.' && p[1] == '.')) {
            RemoveLastDir(fullPath);
            ++p;
        } else if (*p == '/') {
        } else {
            strcat(fullPath, "/");
            strncat(fullPath, p, strchr(p, '/') - p);
            p = strchr(p, '/');
            if (p == NULL) break;
        }
    }

    return fullPath;
}

void RemoveLastDir(char *path) {
    char *p = strrchr(path, '/');

    memset(p, 0, strlen(p));
}
