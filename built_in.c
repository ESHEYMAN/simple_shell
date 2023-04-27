#include "main.h"

/**
 * buitn_env - implement the environment builtin
 * @info: General information about shell
 * @arguments: commands.
 *
 */

void buitn_env(general_t *info, char **arguments)
{
	(void) info;
	(void) arguments;

	get_all_environ();
}
