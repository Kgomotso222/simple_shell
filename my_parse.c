#include "shell.h"

/**
 * is_custom_command - determines if a file is an executable command
 * @info: the custom_info_t struct
 * @path: path to the file
 *
 * Return: (1) if true, (0) otherwise
 */
int is_custom_command(custom_info_t *info, char *path)
{
    struct stat st;

    (void)info;
    if (!path || stat(path, &st))
        return (0);

    if (st.st_mode & S_IFREG)
    {
        return (1);
    }
    return (0);
}

/**
 * duplicate_custom_string - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *duplicate_custom_string(char *pathstr, int start, int stop)
{
    static char buf[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
        if (pathstr[i] != ':')
            buf[k++] = pathstr[i];
    buf[k] = 0;
    return (buf);
}

/**
 * locate_executable_path - finds this cmd in the PATH string
 * @info: the custom_info_t struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *locate_executable_path(custom_info_t *info, char *pathstr, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!pathstr)
        return (NULL);
    if ((custom_strlen(cmd) > 2) && custom_starts_with(cmd, "./"))
    {
        if (is_custom_command(info, cmd))
            return (cmd);
    }
    while (1)
    {
        if (!pathstr[i] || pathstr[i] == ':')
        {
            path = duplicate_custom_string(pathstr, curr_pos, i);
            if (!*path)
                custom_strcat(path, cmd);
            else
            {
                custom_strcat(path, "/");
                custom_strcat(path, cmd);
            }
            if (is_custom_command(info, path))
                return (path);
            if (!pathstr[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return (NULL);
}
