# Shell

### Execute

Make command: ```make all``` <br>
Run command: ```./shell```

### Files
1. libs/builtins/builtin.h: commands ```echo```, ```pwd``` and ```cd```
2. libs/history/history.h: command ```history```
3. libs/ls/ls.h: command ```ls```
4. libs/nightswatch/nightswatch.h: command ```nightswatch```
5. libs/other_commands/othercommands.h: All backgroun and foreground commands that are implemented using ```execvp```
6. libs/pinfo/pinfo.h: command ```pinfo```
7. utils/getinput.h: all functions required to separate commands and tokenize the arguments
8. utils/global.h: all global variables
9. shell.c: the main shell loop and shell prompt

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
5. Shell exits on typing ```exit```, just like in bash
6. cd does not get fooled by random tildas or dots, mimicked exactly from bash