#include "shell.h"

/**
 * custom_shell - main shell loop
 * @info: the custom_info_t struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_shell(custom_info_t *info, char **av)
{
    ssize_t r = 0;
    int builtin_ret = 0;

    while (r != -1 && builtin_ret != -2)
    {
        clear_custom_info(info);
        if (interactive(info))
            custom_puts("$ ");
        custom_eputchar(BUF_FLUSH);
        r = get_input(info);
        if (r != -1)
        {
            set_custom_info(info, av);
            builtin_ret = identify_builtin_command(info);
            if (builtin_ret == -1)
                locate_command(info);
        }
        else if (interactive(info))
            custom_putchar('\n');
        free_custom_info(info, 0);
    }
    write_custom_history(info);
    free_custom_info(info, 1);
    if (!interactive(info) && info->status)
        exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return (builtin_ret);
}

/**
 * identify_builtin_command - identifies a builtin command
 * @info: the custom_info_t struct
 *
 * Return: -1 if builtin not found,
 * 	0 if builtin executed successfully,
 * 	1 if builtin found but not successful,
 * 	2 if builtin signals exit()
 */
int identify_builtin_command(custom_info_t *info)
{
    int i, builtin_ret = -1;
    builtin_table custom_builtintbl[] = {
        {"exit", custom_exit},
        {"env", custom_env},
        {"help", custom_help},
        {"history", custom_history},
        {"setenv", custom_setenv},
        {"unsetenv", custom_unsetenv},
        {"cd", custom_cd},
        {"alias", custom_alias},
        {NULL, NULL}
    };

    for (i = 0; custom_builtintbl[i].type; i++)
        if (custom_strcmp(info->argv[0], custom_builtintbl[i].type) == 0)
        {
            info->line_count++;
            builtin_ret = custom_builtintbl[i].func(info);
            break;
        }
    return (builtin_ret);
}

/**
 * locate_command - locates a command in PATH
 * @info: the custom_info_t struct
 *
 * Return: void
 */
void locate_command(custom_info_t *info)
{
    char *path = NULL;
    int i, k;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0, k = 0; info->arg[i]; i++)
        if (!is_custom_delim(info->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = locate_executable_path(info, custom_getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        execute_command(info);
    }
    else
    {
        if ((interactive(info) || custom_getenv(info, "PATH=")
                    || info->argv[0][0] == '/') && is_custom_command(info, info->argv[0]))
            execute_command(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            custom_print_error(info, "not found\n");
        }
    }
}

/**
 * execute_command - executes a command
 * @info: the custom_info_t struct
 *
 * Return: void
 */
void execute_command(custom_info_t *info)
{
    pid_t child_pid;

    child_pid = custom_fork();
    if (child_pid == -1)
    {
        custom_perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (custom_execve(info->path, info->argv, get_custom_environ(info)) == -1)
        {
            free_custom_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                custom_print_error(info, "Permission denied\n");
        }
    }
}
