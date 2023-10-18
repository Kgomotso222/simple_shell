#include "shell.h"

/**
 * add_custom_node - adds a node to the start of the custom_list
 * @head: address of pointer to the head node
 * @str: str field of the node
 * @num: node index used by history
 *
 * Return: the new head of the custom_list
 */
custom_list_t *add_custom_node(custom_list_t **head, const char *str, int num)
{
    custom_list_t *new_head;

    if (!head)
        return (NULL);
    new_head = malloc(sizeof(custom_list_t));
    if (!new_head)
        return (NULL);
    memset((void *)new_head, 0, sizeof(custom_list_t));
    new_head->num = num;
    if (str)
    {
        new_head->str = strdup(str);
        if (!new_head->str)
        {
            free(new_head);
            return (NULL);
        }
    }
    new_head->next = *head;
    *head = new_head;
    return (new_head);
}

/**
 * add_custom_node_end - adds a node to the end of the custom_list
 * @head: address of pointer to the head node
 * @str: str field of the node
 * @num: node index used by history
 *
 * Return: the new tail of the custom_list
 */
custom_list_t *add_custom_node_end(custom_list_t **head, const char *str, int num)
{
    custom_list_t *new_node, *node;

    if (!head)
        return (NULL);

    node = *head;
    new_node = malloc(sizeof(custom_list_t));
    if (!new_node)
        return (NULL);
    memset((void *)new_node, 0, sizeof(custom_list_t));
    new_node->num = num;
    if (str)
    {
        new_node->str = strdup(str);
        if (!new_node->str)
        {
            free(new_node);
            return (NULL);
        }
    }
    if (node)
    {
        while (node->next)
            node = node->next;
        node->next = new_node;
    }
    else
        *head = new_node;
    return (new_node);
}

/**
 * print_custom_list_str - prints only the str element of a custom_list linked list
 * @head: pointer to the first node
 *
 * Return: size of the custom_list
 */
size_t print_custom_list_str(const custom_list_t *head)
{
    size_t i = 0;

    while (head)
    {
        puts(head->str ? head->str : "(nil)");
        puts("\n");
        head = head->next;
        i++;
    }
    return (i);
}

/**
 * delete_custom_node_at_index - deletes a node at the given index
 * @head: address of pointer to the head node
 * @index: index of the node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_custom_node_at_index(custom_list_t **head, unsigned int index)
{
    custom_list_t *node, *prev_node;
    unsigned int i = 0;

    if (!head || !*head)
        return (0);

    if (!index)
    {
        node = *head;
        *head = (*head)->next;
        free(node->str);
        free(node);
        return (1);
    }
    node = *head;
    while (node)
    {
        if (i == index)
        {
            prev_node->next = node->next;
            free(node->str);
            free(node);
            return (1);
        }
        i++;
        prev_node = node;
        node = node->next;
    }
    return (0);
}

/**
 * custom_free_list - frees all nodes of a custom_list
 * @head_ptr: address of pointer to the head node
 *
 * Return: void
 */
void custom_free_list(custom_list_t **head_ptr)
{
    custom_list_t *node, *next_node, *head;

    if (!head_ptr || !*head_ptr)
        return;
    head = *head_ptr;
    node = head;
    while (node)
    {
        next_node = node->next;
        free(node->str);
        free(node);
        node = next_node;
    }
    *head_ptr = NULL;
}
