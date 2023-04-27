#include "main.h"

/**
 * analyze - analyze argument
 * @arguments: arguments and commands to execute.
 * @info: general informations about the shell.
 * @buff: lines re-added
 */

void analyze(char **arguments, char *buff, general_t *info)
{
	int status;
	char *cmd;

	if (**arguments == NULL || arguments == NULL)
		return;

	cmd = arguments[0];
	info->command = cmd;

	if (access(info, arguments) == TRUE)
		return;

	status = file(cmd);

	if (status == NON_PERMISSION)
	{
		info->status_code = 130;
		info->status_code = CODE_ACCESS;
		error(info);
		return;
	}

	if (status == 1)
	{
		execve(arguments, buff, info, cmd);
		return;
	}

	if (current_dir(arguments, buff, info, cmd) == TRUE)
		return;

	info->value_path = which(info, cmd);
	if (info->value_path != NULL)
	{
		execve(arguments, buff, info, info->value_path);
		free_memory((void *) info->value_path);
		return;
	}

	info->value_code = 131;
	info->error_code = CODE_CMD_DONT_EXIST;
	error(info);
}
