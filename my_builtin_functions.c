#include "shell.h"

/**
 * custom_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: exits with a given exit status (0) if info->argument_vector[0] != "exit"
 */
int custom_exit(custom_info_t *info)
{
    int exitcheck;

    if (info->argument_vector[1])  /* If there is an exit argument */
    {
        exitcheck = custom_atoi(info->argument_vector[1]);
        if (exitcheck == -1)
        {
            info->return_status = 2;
            custom_puts("Illegal number: ");
            custom_puts(info->argument_vector[1]);
            custom_putchar('\n');
            return (1);
        }
        info->error_count = custom_atoi(info->argument_vector[1]);
        return (-2);
    }
    info->error_count = -1;
    return (-2);
}

/**
 * custom_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: Always 0
 */
int custom_cd(custom_info_t *info)
{
    char *s, *dir, buffer[1024];
    int chdir_ret;

    s = getcwd(buffer, 1024);
    if (!s)
        custom_puts("TODO: >>getcwd failure emsg here<<\n");
    if (!info->argument_vector[1])
    {
        dir = get_custom_env(info, "HOME=");
        if (!dir)
            chdir_ret = chdir((dir = get_custom_env(info, "PWD=")) ? dir : "/");
        else
            chdir_ret = chdir(dir);
    }
    else if (custom_strcmp(info->argument_vector[1], "-") == 0)
    {
        if (!get_custom_env(info, "OLDPWD="))
        {
            custom_puts(s);
            custom_putchar('\n');
            return (1);
        }
        custom_puts(get_custom_env(info, "OLDPWD="));
        custom_putchar('\n');
        chdir_ret = chdir((dir = get_custom_env(info, "OLDPWD=")) ? dir : "/");
    }
    else
        chdir_ret = chdir(info->argument_vector[1]);
    if (chdir_ret == -1)
    {
        print_custom_error(info, "can't cd to ");
        custom_puts(info->argument_vector[1]);
        custom_putchar('\n');
    }
    else
    {
        set_custom_env(info, "OLDPWD", get_custom_env(info, "PWD="));
        set_custom_env(info, "PWD", getcwd(buffer, 1024));
    }
    return (0);
}

/**
 * custom_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: Always 0
 */
int custom_help(custom_info_t *info)
{
    char **arg_array;

    arg_array = info->argument_vector;
    custom_puts("help call works. Function not yet implemented \n");
    if (0)
        custom_puts(*arg_array); /* temp att_unused workaround */
    return (0);
}
