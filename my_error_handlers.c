#include "shell.h"

/**
 * custom_puts - prints an input string.
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void custom_puts(char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i] != '\0')
    {
        custom_putchar(str[i]);
        i++;
    }
}

/**
 * custom_putchar - writes the character 'c' to the specified file descriptor.
 * @c: The character to print
 *
 * Return: On success, 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int custom_putchar(char c)
{
    static int i;
    static char buf[CUSTOM_WRITE_BUFFER_SIZE];

    if (c == CUSTOM_BUFFER_FLUSH || i >= CUSTOM_WRITE_BUFFER_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != CUSTOM_BUFFER_FLUSH)
        buf[i++] = c;
    return (1);
}

/**
 * write_to_fd - writes the character 'c' to the specified file descriptor.
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success, 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_to_fd(char c, int fd)
{
    static int i;
    static char buf[CUSTOM_WRITE_BUFFER_SIZE];

    if (c == CUSTOM_BUFFER_FLUSH || i >= CUSTOM_WRITE_BUFFER_SIZE)
    {
        write(fd, buf, i);
        i = 0;
    }
    if (c != CUSTOM_BUFFER_FLUSH)
        buf[i++] = c;
    return (1);
}

/**
 * write_str_to_fd - writes the string 'str' to the specified file descriptor.
 * @str: The string to print
 * @fd: The file descriptor to write to
 *
 * Return: On success, the number of characters written.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_str_to_fd(char *str, int fd)
{
    int i = 0;

    if (!str)
        return (-1);
    while (str[i] != '\0')
    {
        write_to_fd(str[i], fd);
        i++;
    }
    return (i);
}
