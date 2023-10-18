#include "shell.h"

/**
 * custom_env - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int custom_env(custom_info_t *info)
{
	print_custom_list_str(info->environment_list);
	return (0);
}

/**
 * get_custom_env - gets the value of a custom environment variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: custom environment variable name
 *
 * Return: the value
 */
char *get_custom_env(custom_info_t *info, const char *name)
{
	custom_list_t *node = info->environment_list;
	char *p;

	while (node)
	{
		p = custom_node_starts_with(node->data, name, '=');
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * set_custom_env - Initialize a new custom environment variable,
 *                 or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int set_custom_env(custom_info_t *info)
{
	if (info->argument_count != 3)
	{
		custom_puts("Incorrect number of arguments\n");
		return (1);
	}
	if (set_custom_env(info, info->argument_vector[1], info->argument_vector[2]))
		return (0);
	return (1);
}

/**
 * unset_custom_env - Remove a custom environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int unset_custom_env(custom_info_t *info)
{
	int i;

	if (info->argument_count == 1)
	{
		custom_puts("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i < info->argument_count; i++)
		unset_custom_env(info, info->argument_vector[i]);

	return (0);
}

/**
 * populate_custom_env_list - populates custom environment linked list
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int populate_custom_env_list(custom_info_t *info)
{
	custom_list_t *node = NULL;
	size_t i;

	for (i = 0; custom_environment[i]; i++)
		add_custom_node_end(&node, custom_environment[i], 0);
	info->environment_list = node;
	return (0);
}
