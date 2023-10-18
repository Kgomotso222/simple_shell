#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
    custom_info_t info;
    custom_info_init(&info);

    if (ac == 2)
    {
        int fd = open(av[1], O_RDONLY);
        if (fd == -1)
        {
            if (errno == EACCES)
                return 126;
            if (errno == ENOENT)
            {
                custom_eputs(av[0]);
                custom_eputs(": 0: Can't open ");
                custom_eputs(av[1]);
                custom_eputchar('\n');
                custom_eputchar(BUF_FLUSH);
                return 127;
            }
            return EXIT_FAILURE;
        }
        info.readfd = fd;
    }

    populate_custom_env_list(&info);
    read_history_custom(&info);
    hsh(&info, av);
    return EXIT_SUCCESS;
}
