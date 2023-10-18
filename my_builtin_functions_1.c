#include "shell.h"

/**
 * custom_history - displays the history list, one command by line, preceded
 * with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int custom_history(custom_info_t *info)
{
	print_custom_list_str(info->history_node);
	return (0);
}

/**
 * unset_custom_alias - unsets an alias
 * @info: parameter struct
 * @str: the string alias
 * Return: Always 0 on success, 1 on error
 */
int unset_custom_alias(custom_info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = custom_strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_custom_node_at_index(&(info->alias_node),
		get_custom_node_index(info->alias_node, custom_node_starts_with(info->alias_node, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_custom_alias - sets an alias
 * @info: parameter struct
 * @str: the string alias
 * Return: Always 0 on success, 1 on error
 */
int set_custom_alias(custom_info_t *info, char *str)
{
	char *p;

	p = custom_strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_custom_alias(info, str));

	unset_custom_alias(info, str);
	return (add_custom_node_end(&(info->alias_node), str, 0) == NULL);
}

/**
 * print_custom_alias - prints an alias string
 * @node: the alias node
 * Return: Always 0 on success, 1 on error
 */
int print_custom_alias(custom_list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = custom_strchr(node->data, '=');
		for (a = node->data; a <= p; a++)
			custom_putchar(*a);
		custom_putchar('\'');
		custom_puts(p + 1);
		custom_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * custom_alias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int custom_alias(custom_info_t *info)
{
	int i = 0;
	char *p = NULL;
	custom_list_t *node = NULL;

	if (info->argument_count == 1)
	{
		node = info->alias_node;
		while (node)
		{
			print_custom_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argument_vector[i]; i++)
	{
		p = custom_strchr(info->argument_vector[i], '=');
		if (p)
			set_custom_alias(info, info->argument_vector[i]);
		else
			print_custom_alias(custom_node_starts_with(info->alias_node, info->argument_vector[i], '='));
	}

	return (0);
}
