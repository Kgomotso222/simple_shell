#include "shell.h"

/**
 * get_custom_history_file - gets the custom history file
 * @info: custom_info_t struct
 *
 * Return: allocated string containing custom history file
 */
char *get_custom_history_file(custom_info_t *info)
{
	char *buf, *dir;

	dir = get_custom_env(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = custom_malloc(sizeof(char) * (custom_strlen(dir) + custom_strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	custom_strcpy(buf, dir);
	custom_strcat(buf, "/");
	custom_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_custom_history - creates a file, or appends to an existing file
 * @info: the custom_info_t struct
 *
 * Return: 1 on success, else -1
 */
int write_custom_history(custom_info_t *info)
{
	ssize_t fd;
	char *filename = get_custom_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = custom_open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	custom_free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->custom_history; node; node = node->next)
	{
		custom_putsfd(node->str, fd);
		custom_putfd('\n', fd);
	}
	custom_putfd(CUSTOM_BUF_FLUSH, fd);
	custom_close(fd);
	return (1);
}

/**
 * read_custom_history - reads custom history from a file
 * @info: the custom_info_t struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_custom_history(custom_info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_custom_history_file(info);

	if (!filename)
		return (0);

	fd = custom_open(filename, O_RDONLY);
	custom_free(filename);
	if (fd == -1)
		return (0);
	if (!custom_fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = custom_malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = custom_read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return custom_free(buf), 0;
	custom_close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			custom_build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		custom_build_history_list(info, buf + last, linecount++);
	custom_free(buf);
	info->custom_histcount = linecount;
	while (info->custom_histcount-- >= HIST_MAX)
		custom_delete_node_at_index(&(info->custom_history), 0);
	custom_renumber_history(info);
	return info->custom_histcount;
}

/**
 * custom_build_history_list - adds an entry to a custom history linked list
 * @info: custom_info_t struct
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int custom_build_history_list(custom_info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->custom_history)
		node = info->custom_history;
	custom_add_node_end(&node, buf, linecount);

	if (!info->custom_history)
		info->custom_history = node;
	return 0;
}

/**
 * custom_renumber_history - renumbers the custom history linked list after changes
 * @info: custom_info_t struct
 *
 * Return: the new histcount
 */
int custom_renumber_history(custom_info_t *info)
{
	list_t *node = info->custom_history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->custom_histcount = i);
}
