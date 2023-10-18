#include "shell.h"

/**
 * clear_custom_info - initializes custom_info_t struct
 * @info: struct address
 */
void clear_custom_info(custom_info_t *info)
{
    info->custom_arg = NULL;
    info->custom_argv = NULL;
    info->custom_path = NULL;
    info->custom_argc = 0;
}

/**
 * set_custom_info - initializes custom_info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_custom_info(custom_info_t *info, char **av)
{
    int i = 0;

    info->custom_fname = av[0];
    if (info->custom_arg)
    {
        info->custom_argv = custom_strtow(info->custom_arg, " \t");
        if (!info->custom_argv)
        {
            info->custom_argv = custom_malloc(sizeof(char *) * 2);
            if (info->custom_argv)
            {
                info->custom_argv[0] = custom_strdup(info->custom_arg);
                info->custom_argv[1] = NULL;
            }
        }
        for (i = 0; info->custom_argv && info->custom_argv[i]; i++)
            ;
        info->custom_argc = i;

        custom_replace_alias(info);
        custom_replace_vars(info);
    }
}

/**
 * free_custom_info - frees custom_info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_custom_info(custom_info_t *info, int all)
{
    custom_ffree(info->custom_argv);
    info->custom_argv = NULL;
    info->custom_path = NULL;
    if (all)
    {
        if (!info->custom_cmd_buf)
            custom_free(info->custom_arg);
        if (info->custom_env)
            custom_free_list(&(info->custom_env));
        if (info->custom_history)
            custom_free_list(&(info->custom_history));
        if (info->custom_alias)
            custom_free_list(&(info->custom_alias));
        custom_ffree(info->custom_environ);
        info->custom_environ = NULL;
        custom_bfree((void **)info->custom_cmd_buf);
        if (info->custom_readfd > 2)
            close(info->custom_readfd);
        custom_putchar(CUSTOM_BUF_FLUSH);
    }
}
