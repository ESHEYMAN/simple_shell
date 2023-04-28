#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

/* This is a pointer that point to the string environ */
extern char **environ;

/**
 * struct repeat_var_list - this is a linked list struct
 * @num: this is use to store the len of the variable
 * @data: this store the data of the var
 * @len_data: this store the len of the data
 * @next: this is a pointer that point to the next struct
 */
typedef struct repeat_var_list
{
	int num;
	char *data;
	int len_data;
	struct repeat_var_list *next;
} repeat_var;

/**
 * struct info - struct for the program data
 * @program_name: the name of the executable
 * @input_line: pointer to the input read for_getline
 * @command_name: pointer to the first command typed by the user
 * @exec_counter: number of executed commands
 * @file_descriptor: file descriptor to the input of commands
 * @token: pointer to the array of tokenize input
 * @env: copy of the environ
 * @alias_list: array of pointers with aliases
 */
typedef struct info
{
	char *program_name;
	char *input_line;
	char *command_name;
	char **tokens;
	char **env;
	char **alias_list;
	int file_descriptor;
	int exec_counter;
} data_of_program;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
}
info_t;

typedef struct __attribute__((__packed__))
{
	int argc;                 /* Number of arguments received */
	char **argv;              /* Arguments received */
	int mode;                 /* INTERACTIVE or NON_INTERACTIVE */
	int error_code;           /* Error code for error message */
	char *command;            /* Command to execute */
	int n_commands;           /* Number of commands executed */
	char *value_path;         /* Path of a command */
	int is_current_path;      /* Check if is current path or not */
	int status_code;          /* Last exit code */
	char *buffer;             /* Line readed with the getline */
	char **arguments;         /* Line splited into words */
	char *environment;        /* Last environment variable get it */
	int pid;                  /* Process id */
} general_t;

/**
 * struct line_sep - this is a linked list that checks the operator
 * @sep_operator: this char checks for the operator
 * @next: a pointer that points to the next node
 * Description: a linked list that checks and hold the operator
 */
typedef struct line_sep
{
	char sep_operator;
	struct line_sep *next;
}
cmd_sep;

/**
 * struct builtins - struct for the builtins
 * @type: the name of the builtin
 * @function: the function
 */
typedef struct builtin
{
	int (*funct)(info_t *);
	char *type;
}
builtin_table;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
}
list_t;

/**
 * struct shell data - used to store response and input from the user
 * @argv: the array of argument vector of string
 * @argc: represent the number of argument count
 * @args: represent the token passed from the command line
 * @environ: represent the env variable passed
 * @shdata: represent the shell input based by the user  command line
 * @status: used to store the program process
 * @pid: represent the process pid
 *
 * Description: struct is created to store the respose,token,
 * env path from the user
 */
typedef struct shell_data
{
	char **argv;
	char **args;
	char **environ;
	char *pid;
	char *shdata;
	int argc;
	int status;
}
data_shell;

/**
 * struct based_s - this is a inbuilt struct fot the shell
 * @cmd_name: represent the command name of the inbuilt struct
 * @flag: represent the data type of the function
 */
typedef struct based_s
{
	char *cmd_name;
	int (*flag)(data_shell *data);
}
based_sys;

/**
 * initialise the based_sys with an array of input coomand
 * with the core function name
 */
#define BASED_SYS_INIT \
{ \
	{"env", _env_path}, \
	{"exit", _exit_sh}, \
	{"setenv", _set_env}, \
	{"unsetenv", _unset_env}, \
	{"cd", _cd_sh}, \
	{NULL, NULL} \
}



/* main functions */

void find_cmd(info_t *);
void fork_cmd(info_t *);
int hsh(info_t *, char **);
int find_builtin(info_t *);

/* interpreter and permission */
void analyse(char **arguments, general_t *info, char *buff);

/* permissions.c */
int is_executable(char *filename);
int is_filed(char *command);

/* environment.c */
char *_get_environ(const char *name);
char *what(char *filename, general_t *info);
void is_main_path(char *path, general_t *info);
void get_all_environ(void);

/* execute.c */
void execute(char *commands, char **arguments, general_t *info, char *buff);
int current_dir(char *cmd, char **arguments, char *buff, general_t *info);

int exitmode(info_m *info);
int ccd(info_t *info);
int hhelp(info_t *info);

/* env builtin */
void buitn_env(general_t *info, char **arguments)

#endif
