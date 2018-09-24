#ifndef SHELL_H
#define SHELL_H

void user_input(char *input);

typedef int (*shell_cmd_t)(char*);

#endif
