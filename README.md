# Shell

### Execute

Make command: ```make all``` <br>
Run command: ```./shell```

### Files
1. libs/builtins/builtin.h: Commands ```echo```, ```pwd``` and ```cd```
2. libs/history/history.h: Command ```history```
3. libs/ls/ls.h: command ```ls```
4. libs/nightswatch/nightswatch.h: Command ```nightswatch```
5. libs/nightswatch/nw_interrupts.h: The interrupts option of ```nightswatch```
6. libs/nightswatch/nw_newborn.h: The newborn option of ```nightswatch``` 
7. libs/other_commands/othercommands.h: All background and foreground commands are called from here
8. libs/other_commands/foregroundprocesses.h: Implementation for calling processes in the foreground
8. libs/other_commands/backgroundprocesses.h: Implementation for calling processes in the background
9. libs/pinfo/pinfo.h: command ```pinfo```
10. libs/jobs/updatejobs.h: Calls the appropriate function related to jobs
11. libs/jobs/listjobs.h: Command ```jobs```
12. libs/jobs/killall.h: Command ```overkill```
13. libs/jobs/sengsignal.h: Command ```kjob```
14. libs/jobs/startbg.h: Command ```bg```
15. libs/jobs/bgtofg.h: Command ```fg```
16. libs/envupdate/envupdate.h: Command ```env```
17. utils/getinput.h: All functions required to separate commands and tokenize the arguments
18. utils/global.h: All global variables
19. utils/checkpipes.h: Checks whether the command has piping or not
20. utils/lookup.h: Looks up the function to be called for the current command
21. utils/piping.h: If the command has piping, pipes the commands
22. utils/prompt.h: Prints the command prompt
23. utils/signalhandlers.h: Various signal handlers of the program
24. shell.c: the main shell loop and shell prompt

### Assumptions
1. Shell prompt will not be longer than 2048 characters.
2. Path wont be longer than 1024 characters.
3. Commands wont have more than 1024 characters.
4. File name is not longer than 1024 characters.
5. Process informations in files do not contain lines longer than 1024 characters.
6. Max number of processes at any point of time is not more than 1024

### Additional features
1. Shell prompt is in green color
2. History tells the command number of the previous 20 commands
3. Multiple commands separated with ";" are written separately in history!
4. Background processes are indexed, just like in bash
5. cd does not get fooled by random tildas or dots, mimicked exactly from bash
6. <b>Process that have been terminated and removed are still displayed in the jobs list</b>
7. A command with first character ```$``` and only 1 argument is an environment variable that is to be printed. The environment variable does infact exist. E.g writing ```$PATH``` prints the PATH environment variable