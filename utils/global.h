#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

#include <string.h>

// Constraints
#define INPUT_LENGTH 1024
#define MAX_PATH_LENGTH 1024
#define MAX_SHELL_PROMPT_LENGTH 2048
#define MAX_FILE_INFO 1024
#define MAX_PROCESS_INFO 1024
const int MAX_ARGUMENTS = 1024;
const char *DELIMITER = " \t\r\n\a";
#define MAX_PROCESSES 1024

// Utility
char **multiargs;
char **args;
int numargs;
int numCommands;
char shellPath[MAX_PATH_LENGTH]; //Home path of shell
int shellPID;
int curForegroundProcess;
char ret[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int commandCnt = 0;
char processesName[MAX_PROCESSES][MAX_PROCESS_INFO];
int processesID[MAX_PROCESSES];
char historyFilePath[128] = "/.shell_history.txt";
int copyOfStdout;
int changedStdOut;
int copyOfStdIn;
int changedStdIn;
char curPath[MAX_PATH_LENGTH];        // Current path of shell
char prompt[MAX_SHELL_PROMPT_LENGTH]; // Prompt
char *inp;

// Errors
char MEMORY_ALLOCATION_ERROR[] = "Memorry allocation error";
char PATH_SIZE_ERROR[] = "Memorry allocation error";

// Functions
int checkShellHome(char *oth)
{
    int x = strlen(oth);
    int y = strlen(shellPath);
    if (x < y)
        return 0;
    for (int a = 0; a < y; a++)
        if (oth[a] != shellPath[a])
            return 0;
    return 1;
}
#endif