#include "shell.h"
/**
 * interactive_custom - Check if the shell is in interactive mode.
 * @info: A pointer to the custom_info_t struct.
 *
 * Return: 1 if the shell is in interactive mode, 0 otherwise.
 */
int interactive_custom(custom_info_t *info)
{
    return (isatty(STDIN_FILENO) && info->read_fd <= 2);
}
/**
 * is_custom_delimiter - Check if a character is a delimiter.
 * @c: The character to check.
 * @delim: The delimiter string.
 *
 * Return: 1 if the character is a delimiter, 0 if it is not.
 */
int is_custom_delimiter(char c, char *delim)
{
    while (*delim)
        if (*delim++ == c)
            return (1);
    return (0);
}
/**
 * custom_is_alpha - Check if a character is alphabetic.
 * @c: The character to check.
 *
 * Return: 1 if the character is alphabetic, 0 otherwise.
 */
int custom_is_alpha(int c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return (1);
    else
        return (0);
}
/**
 * custom_atoi - Convert a string to an integer.
 * @s: The string to be converted.
 *
 * Return: 0 if there are no numbers in the string, the converted number otherwise.
 */
int custom_atoi(char *s)
{
    int i, sign = 1, flag = 0, output;
    unsigned int result = 0;

    for (i = 0; s[i] != '\0' && flag != 2; i++)
    {
        if (s[i] == '-')
            sign *= -1;

        if (s[i] >= '0' && s[i] <= '9')
        {
            flag = 1;
            result *= 10;
            result += (s[i] - '0');
        }
        else if (flag == 1)
            flag = 2;
    }

    if (sign == -1)
        output = -result;
    else
        output = result;

    return (output);
}
