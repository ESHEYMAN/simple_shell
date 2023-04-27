#include "main.h"

/**
 * analyse - analyse argument
 * @arguments: arguments and commands to execute.
 * @info: general informations about the shell.
 * @buff: lines re-added
 */

void analyse(char **arguments, char *buff, general_t *info)
{
	int status;
	char *comd;

	if (*arguments == NULL || arguments == NULL)
	return (0);

	comd = arguments[0];
	info->command = comd;
	if (check_builtin(info, arguments) == _TRUE)
	return (0);

	status = is_field(comd);
	if (status == NON_PERMISSIONS)
{
	info->status_code = 126;
	info->error_code = _CODE_ACESS;
	error(info);
	return (0);
}
	if (status == 1)
{
	execute(comd, arguments, buff, info);
	return (0);
}

	if (current_dir(comd, arguments, buff, info) == _TRUE)
	return (0);

	info->value_path = which(comd, info);
	if (info->value_path != NULL)
{
	execute(info->value_path, arguments, info, buff);
	free_memory_p((void *) info->value_path);
	return (0);
}
	info->status_code = 127;
	info->error_code = _CODE_COMD_DOES_NOT_EXIST;
	error(info);
}
