#ifndef STARTBG
#define STARTBG

#include "../../utils/global.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int startbg()
{
    if (numargs != 2)
    {
        printf("%s: invalid number of arguments\n", args[0]);
        return 0;
    }
    int len_arg1 = strlen(args[1]);
    for (int a = 0; a < len_arg1; a++)
        if (args[1][a] < 48 || args[1][a] > 57)
        {
            printf("%s: invalid argument\n", args[0]);
            return 0;
        }
    int arg1 = 0;
    for (int a = 0; a < len_arg1; a++)
    {
        arg1 *= 10;
        arg1 += (int)((int)args[1][a] - 48);
    }
    if (arg1 > commandCnt || arg1 == 0)
    {
        printf("%s: no such job\n", args[0]);
        return 0;
    }
    int x = kill(processesID[arg1 - 1], SIGCONT);
    if (x < 0)
    {
        printf("Error in sending SIGCONT\n");
        return 0;
    }
    return 1;
}

#endif

/*
#include "headers.h"

extern char homepath[], curpath[] ; 
extern int processes[], processCnt ;

int sigintFlag = 0, sigtstpFlag = 0, pid ; 

void sigtstpHandler(int sig) {
	kill(pid, SIGTSTP); 
	// processes[processCnt++] = pid ; 
}

void sigintHandler(int sig) {
	kill(pid, SIGINT); 
	return ;
}

void handle_fg(char* command) {
	char* token = strtok(command, " \t");
	char* jobnum = strtok(NULL, " \t");

	if(jobnum == NULL) {
        perror("Usage: fg [job number]");
        return;
	}
	// handle jobnum if it is not integer!! 

	int job = atoi(jobnum);
	int i, exists = 0, cnt = 0 ;
	for(i = 0; i < processCnt; i++) {
		if(processes[i] < 0) continue ; 
		if(job == ++cnt) {
			exists = 1 ; 
			int status ; pid = processes[i] ;
			signal(SIGINT, sigintHandler); signal(SIGTSTP, sigtstpHandler); 
			// resume bg process into foregroung and wait for it untill interrupted 
			kill(pid, SIGCONT);
			waitpid(pid, &status, WUNTRACED);
			break ;
		}
	}
	if(!exists) perror("Given job does not exist!"); 

}

void handle_bg(char* command) {
	char* token = strtok(command, " \t");
	char* jobnum = strtok(NULL, " \t");

	if(jobnum == NULL) {
        perror("Usage: fg [job number]");
        return;
	}
	// handle jobnum if it is not integer!! 

	int job = atoi(jobnum);
	int i, exists = 0, cnt = 0 ;
	for(i = 0; i < processCnt; i++) {
		if(processes[i] < 0) continue ; 
		if(job == ++cnt) {
			exists = 1 ; 
			// SIGSTOP for stopping and SIGCONT for resuming 
			if( kill(processes[i], SIGCONT) < 0) {
				perror("Error in changing the state of the process to running the given job");
				return ; 
			}
		}
	}
	if(!exists) perror("Given job does not exist!"); 
}
*/