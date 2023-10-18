#include "shell.h"

/**
 * get_custom_environ - returns a copy of the custom environment variables as an array of strings
 * @info: Pointer to the custom_info_t structure
 * Return: An array of strings containing custom environment variables
 */
char **get_custom_environ(custom_info_t *info)
{
    if (!info->custom_environment || info->environment_changed)
    {
        info->custom_environment = custom_list_to_strings(info->environment_list);
        info->environment_changed = 0;
    }

    return (info->custom_environment);
}

/**
 * unset_custom_env - Remove a custom environment variable
 * @info: Pointer to the custom_info_t structure
 * @var: The name of the custom environment variable to remove
 * Return: 1 on success, 0 if the variable was not found
 */
int unset_custom_env(custom_info_t *info, char *var)
{
    custom_list_t *node = info->environment_list;
    custom_list_t *prev = NULL;

    if (!node || !var)
        return (0);

    while (node)
    {
        if (strcmp(node->data, var) == 0)
        {
            if (prev)
                prev->next = node->next;
            else
                info->environment_list = node->next;

            custom_free_list(&node);
            info->environment_changed = 1;
            return (1);
        }

        prev = node;
        node = node->next;
    }

    return (0);
}

/**
 * set_custom_env - Set or modify a custom environment variable
 * @info: Pointer to the custom_info_t structure
 * @var: The name of the custom environment variable to set or modify
 * @value: The value to assign to the custom environment variable
 * Return: 0 on success, -1 on failure
 */
int set_custom_env(custom_info_t *info, char *var, char *value)
{
    if (!var || !value)
        return (-1);

    custom_list_t *node = info->environment_list;

    while (node)
    {
        if (strcmp(node->data, var) == 0)
        {
            custom_free(&node->data);
            node->data = custom_strdup(var);
            if (!node->data)
                return (-1);

            custom_strncat(node->data, "=", 1);
            custom_strncat(node->data, value, custom_strlen(value));
            info->environment_changed = 1;
            return (0);
        }
        node = node->next;
    }

    char *new_var = custom_strdup(var);
    if (!new_var)
        return (-1);

    custom_strncat(new_var, "=", 1);
    custom_strncat(new_var, value, custom_strlen(value));
    add_custom_node_end(&info->environment_list, new_var, 0);
    info->environment_changed = 1;
    return (0);
}
