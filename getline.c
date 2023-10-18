#include "shell.h"

/**
 * get_input_custom - buffers chained commands
 * @info: the custom_info_t parameter struct
 *
 * Return: the number of bytes read
 */
ssize_t get_input_custom(custom_info_t *info) {
    static char *buf; /* The ';' command chain buffer */
    static size_t i, j, len;
    ssize_t r = 0;
    char **buf_p = &(info->argument_vector), *p;

    custom_putchar(CUSTOM_BUFFER_FLUSH);
    r = input_buf_custom(info, &buf, &len);
    if (r == -1) /* EOF */
        return -1;
    if (len) { /* We have commands left in the chain buffer */
        j = i; /* Initialize a new iterator to the current buffer position */
        p = buf + i; /* Get a pointer for return */

        check_chain(info, buf, &j, i, len);
        while (j < len) { /* Iterate to semicolon or end */
            if (is_custom_chain(info, buf, &j))
                break;
            j++;
        }

        i = j + 1; /* Increment past the nulled ';'' */
        if (i >= len) { /* Reached the end of the buffer? */
            i = len = 0; /* Reset position and length */
            info->buffer_type = CUSTOM_COMMAND_NORMAL;
        }

        *buf_p = p; /* Pass back pointer to current command position */
        return custom_strlen(p); /* Return length of current command */
    }

    *buf_p = buf; /* Not a chain, pass back buffer from custom_getline() */
    return r; /* Return length of buffer from custom_getline() */
}

/**
 * input_buf_custom - gets a line minus the newline
 * @info: the custom_info_t parameter struct
 * @buf: the address of the buffer
 * @len: the address of the len variable
 *
 * Return: the number of bytes read
 */
ssize_t input_buf_custom(custom_info_t *info, char **buf, size_t *len) {
    ssize_t r = 0;
    size_t len_p = 0;

    if (!*len) { /* If there's nothing left in the buffer, fill it */
        free(*buf);
        *buf = NULL;
        signal(SIGINT, handle_sigint_custom);
#if CUSTOM_USE_GETLINE
        r = custom_getline(buf, &len_p, stdin);
#else
        r = custom_getline(info, buf, &len_p);
#endif
        if (r > 0) {
            if ((*buf)[r - 1] == '\n') {
                (*buf)[r - 1] = '\0'; /* Remove the trailing newline */
                r--;
            }
            info->count_flag = 1;
            remove_comments(*buf);
            build_custom_history_list(info, *buf, info->history_count++);
            /* if (custom_strchr(*buf, ';')) Is this a command chain? */
            {
                *len = r;
                info->buffer_ptr = buf;
            }
        }
    }
    return r;
}

/**
 * custom_getline - reads a buffer
 * @info: the custom_info_t parameter struct
 * @ptr: the address of a pointer to the buffer, preallocated or NULL
 * @length: the size of preallocated ptr buffer if not NULL
 *
 * Return: the number of bytes read
 */
ssize_t custom_getline(custom_info_t *info, char **ptr, size_t *length) {
    static char buf[CUSTOM_READ_BUFFER_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length)
        s = *length;
    if (i == len)
        i = len = 0;

    r = read_buf_custom(info, buf, &len);
    if (r == -1 || (r == 0 && len == 0))
        return -1;

    c = custom_strchr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_p = custom_realloc(p, s, s ? s + k : k + 1);
    if (!new_p) { /* MALLOC FAILURE! */
        return p ? custom_free((char **)p), -1 : -1;
    }

    if (s) {
        custom_strncat(new_p, buf + i, k - i);
    } else {
        custom_strncpy(new_p, buf + i, k - i + 1);
    }

    s += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = s;
    *ptr = p;
    return s;
}

/**
 * handle_sigint_custom - blocks Ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void handle_sigint_custom(int sig_num) {
    custom_puts("\n");
    custom_puts("$ ");
    custom_putchar(CUSTOM_BUFFER_FLUSH);
}
