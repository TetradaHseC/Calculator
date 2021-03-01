#include <stdbool.h>
#include <printf.h>
#include <stdlib.h>
#include <memory.h>
#include "ui.h"
#include "../res/ui.h"

#define scase(case_) if (strcmp(SWITCH_STRING_ARGUMENT, case_) == 0)

// region console width
int console_width =
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
        #include <sys/ioctl.h>
    #include <stdio.h>
    #include <unistd.h>

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    w.ws_col;
#elif defined(Q_OS_MSDOS)
        #include <windows.h>

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
        80;
#endif
// endregion

char *inputFile = NULL;
char *outputFile = NULL;

enum Commands { InputCommand, OutputCommand, InputRecentCommand, OutputRecentCommand, StartCommand, ExitCommand, NotACommand, ViewRecent };

enum Commands GetCommand();

void PrintInfo();

void EnterFilepath(char **dest);

void OnNewRecent(char *recentStorage, char *newRecent);

void OnGetRecent(char *recentStorage);

void UploadRecent();

void OnViewRecent();

void clear();

void OnInputCommand() {
    EnterFilepath(&inputFile);
    OnNewRecent("input_recent", inputFile);
}

void OnOutputCommand() {
    EnterFilepath(&outputFile);
    OnNewRecent("output_recent", outputFile);
}

void OnInputRecentCommand() {

}

void OnOutputRecentCommand() {

}

void OnNotACommand() {
    printf("Такой команды нет, загляните в инфо сверху\n");
}

int DoUIIteration(char *resultToDraw) {
    bool isDrawResultToDraw = true;
    bool isRunning = true;
    int needToClear = resultToDraw != NULL;

    UploadRecent();

    while (isRunning) {
        if (needToClear == 1) {
            // region console clear
            system(
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
                        "clear"
#elif defined(Q_OS_MSDOS)
                        "cls"
#else
#define NO_CLEAR
                        ""
#endif
            );
#ifdef NO_CLEAR
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#endif
            // endregion
        }
        needToClear = true;

        PrintInfo();
        if (resultToDraw != NULL && isDrawResultToDraw) {
            isDrawResultToDraw = false;
            printf("%s\n  ", resultToDraw);
        }

        switch (GetCommand()) {
            case InputCommand:
                OnInputCommand();
                break;
            case OutputCommand:
                OnOutputCommand();
                break;
            case StartCommand:
                isRunning = false;
                if (strcmp(outputFile, "stdout") == 0)
                    needToClear = false;
                break;
            case ExitCommand:
                free(inputFile);
                free(outputFile);
                exit(0);
            case ViewRecent:
                clear();
                needToClear = 2;
                OnViewRecent();
                break;
            default:
                OnNotACommand();
                break;
        }
        if (needToClear == false) {
            clear();
        }

        system("pause");
    }

    return 0;
}

void clear() {// region console clear
    system(
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
            "clear"
#elif defined(Q_OS_MSDOS)
            "cls"
#else
#define NO_CLEAR
            ""
#endif
    );
#ifdef NO_CLEAR
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#endif
// endregion
}

enum Commands GetCommand() {
    char command[5] = { 0 };
    scanf("%s", command);

#define SWITCH_STRING_ARGUMENT command
    scase("i")
        return InputCommand;
    scase("o")
        return OutputCommand;
    scase("ri")
        return InputRecentCommand;
    scase("ro")
        return OutputRecentCommand;
    scase("s")
        return StartCommand;
    scase("exit")
        return ExitCommand;
    scase("v")
        return ViewRecent;
#undef SWITCH_STRING_ARGUMENT

    return NotACommand;
}

void EnterFilepath(char **dest) {
    printf("Введите название файла(с путём до него, конечно):\n  "); // TODO: res + locals
    char temp[500];
    fflush(stdin);
    gets(temp);
    free(*dest);
    *dest = calloc(strlen(temp) + 1, sizeof(char));
    sprintf(*dest, "%s", temp);
}

void OnNewRecent(char *recentStorage, char *newRecent) {
    char *fullPath = calloc(strlen(recentStorage) + strlen("res/") + 1, sizeof(char));
    strcat(fullPath, "res/");
    strcat(fullPath, recentStorage);
    FILE *storage = fopen(fullPath, "w");
    free(fullPath);

    fprintf(storage, "%s\n", newRecent);
    fclose(storage);
}

void UploadRecent() {
    // region input
    char fullPath[22] = "res/input_recent";
    FILE *storage = fopen(fullPath, "r");

    inputFile = calloc(200, sizeof(char));
    if (storage == NULL) {
        sprintf(inputFile, "None");
        storage = fopen(fullPath, "w");
        fclose(storage);
    } else {
        fgets(inputFile, 200, storage);
        if (strlen(inputFile) > 0) {
            if (inputFile[strlen(inputFile) - 1] == '\n')
                inputFile[strlen(inputFile) - 1] = 0;
        } else
            sprintf(inputFile, "None");
        fclose(storage);
    }
    // endregion

    // region output
    sprintf(fullPath, "%s", "res/output_recent");
    storage = fopen(fullPath, "r");

    outputFile = calloc(200, sizeof(char));
    if (storage == NULL) {
        sprintf(outputFile, "None");
        storage = fopen(fullPath, "w");
        fclose(storage);
    } else {
        fgets(outputFile, 200, storage);
        if (strlen(outputFile) > 0) {
            if (outputFile[strlen(outputFile) - 1] == '\n')
                outputFile[strlen(outputFile) - 1] = 0;
        } else
            sprintf(outputFile, "None");
        fclose(storage);
    }
    // endregion
}

char** _ReadFile(FILE* file, size_t *pcount) {
    char **text = calloc(300, sizeof(char *));
    char *row = calloc(300, sizeof(char));

    int i = 0; // счетчик строк
    while(fgets(row, 300, file) != NULL)
    {
        text[i] = row;
        i++;
        row = calloc(300, sizeof(char));
    }
    free(row);

    *pcount = i;

    return text;
}

void OnViewRecent() {
    FILE *storage = fopen(outputFile, "r");

    if (storage == NULL) {
        printf("Файл не найден или не существует, пожалуйста перезапустите программу или введите имя другого файла.\n");
    } else {
        size_t count = 0;
        char **rows = _ReadFile(storage, &count);

        for (int i = 0; i < count; ++i) {
            printf("%s\n", rows[i]);
        }
    }
    fclose(storage);
}

void PrintInfo() {
    printf(UI_INFO_TEXT,
           inputFile == NULL ? "None" : inputFile,
           outputFile == NULL ? "None" : outputFile);
}

