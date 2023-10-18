#include "shell.h"

/**
 * custom_list_len - determines the length of a custom linked list
 * @h: pointer to the first node
 *
 * Return: size of the custom list
 */
size_t custom_list_len(const custom_list_t *h)
{
    size_t i = 0;

    while (h)
    {
        h = h->next;
        i++;
    }
    return (i);
}

/**
 * custom_list_to_strings - returns an array of strings of the custom list->str
 * @head: pointer to the first node
 *
 * Return: array of strings
 */
char **custom_list_to_strings(custom_list_t *head)
{
    custom_list_t *node = head;
    size_t i = custom_list_len(head), j;
    char **strs;
    char *str;

    if (!head || !i)
        return (NULL);
    strs = malloc(sizeof(char *) * (i + 1));
    if (!strs)
        return (NULL);
    for (i = 0; node; node = node->next, i++)
    {
        str = malloc(_strlen(node->str) + 1);
        if (!str)
        {
            for (j = 0; j < i; j++)
                free(strs[j]);
            free(strs);
            return (NULL);
        }

        str = _strcpy(str, node->str);
        strs[i] = str;
    }
    strs[i] = NULL;
    return (strs);
}

/**
 * custom_print_list - prints all elements of a custom linked list
 * @h: pointer to the first node
 *
 * Return: size of the custom list
 */
size_t custom_print_list(const custom_list_t *h)
{
    size_t i = 0;

    while (h)
    {
        _puts(convert_number(h->index, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(h->str ? h->str : "(nil)");
        _puts("\n");
        h = h->next;
        i++;
    }
    return (i);
}

/**
 * custom_node_starts_with - returns the node whose string starts with the prefix
 * @node: pointer to the custom list head
 * @prefix: string to match
 * @c: the next character after the prefix to match
 *
 * Return: matching node or null
 */
custom_list_t *custom_node_starts_with(custom_list_t *node, char *prefix, char c)
{
    char *p = NULL;

    while (node)
    {
        p = starts_with(node->str, prefix);
        if (p && ((c == -1) || (*p == c)))
            return (node);
        node = node->next;
    }
    return (NULL);
}

/**
 * custom_get_node_index - gets the index of a node
 * @head: pointer to the custom list head
 * @node: pointer to the node
 *
 * Return: the index of the node or -1
 */
ssize_t custom_get_node_index(custom_list_t *head, custom_list_t *node)
{
    size_t i = 0;

    while (head)
    {
        if (head == node)
            return i;
        head = head->next;
        i++;
    }
    return (-1);
}
