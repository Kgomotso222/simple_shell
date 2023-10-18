#ifndef shell_h
#define shell_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* for read/write buffers */
#define CUSTOM_READ_BUFFER_SIZE 1024
#define CUSTOM_WRITE_BUFFER_SIZE 1024
#define CUSTOM_BUFFER_FLUSH -1

/* for command chaining */
#define CUSTOM_COMMAND_NORMAL 0
#define CUSTOM_COMMAND_OR 1
#define CUSTOM_COMMAND_AND 2
#define CUSTOM_COMMAND_CHAIN 3

/* for convert_number() */
#define CUSTOM_CONVERT_LOWERCASE 1
#define CUSTOM_CONVERT_UNSIGNED 2

/* if using system getline() */
#define CUSTOM_USE_GETLINE 0
#define CUSTOM_USE_STRTOK 0

#define CUSTOM_HISTORY_FILE ".custom_shell_history"
#define CUSTOM_HISTORY_MAX 4096

extern char **custom_environment;

/**
 * struct custom_liststr - singly linked list
 * @number: the number field
 * @data: a string
 * @next: points to the next node
 */
typedef struct custom_liststr
{
    int number;
    char *data;
    struct custom_liststr *next;
} custom_list_t;

/**
 * struct custom_passinfo - contains pseudo-arguments to pass into a function,
 * allowing a uniform prototype for a function pointer struct
 * @arguments: a string generated from getline containing arguments
 * @argument_vector: an array of strings generated from arg
 * @current_path: a string path for the current command
 * @argument_count: the argument count
 * @custom_environment: custom modified copy of environment from LL env
 * @environment_changed: on if environment was changed
 * @return_status: the return status of the last exec'd command
 * @buffer_ptr: address of pointer to command buffer, for memory management
 * @buffer_type: COMMAND_type ||, &&, ;
 * @read_fd: the fd from which to read line input
 * @history_count: the history line number count
 * @error_count: the error code for exit()s
 * @count_flag: if on count this line of input
 * @filename: the program filename
 * @environment_list: linked list local copy of environment
 * @history_node: the history node
 * @alias_node: the alias node
 */
typedef struct custom_passinfo
{
    char *arguments;
    char **argument_vector;
    char *current_path;
    int argument_count;
    unsigned int error_count;
    int count_flag;
    char *filename;
    custom_list_t *environment_list;
    custom_list_t *history_node;
    custom_list_t *alias_node;
    char **custom_environment;
    int environment_changed;
    int return_status;

    char **buffer_ptr; /* pointer to command ; chain buffer, for memory management */
    int buffer_type; /* COMMAND_type ||, &&, ; */
    int read_fd;
    int history_count;
} custom_info_t;

#define CUSTOM_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 * struct custom_builtin - contains a custom string and related function
 * @custom_type: the custom command flag
 * @custom_function: the function
 */
typedef struct custom_builtin
{
    char *custom_type;
    int (*custom_function)(custom_info_t *);
} custom_builtin_table;

/* my_shell_loop.c */
int custom_shell(custom_info_t *info, char **av);
int identify_builtin_command(custom_info_t *info);
void locate_command(custom_info_t *info);
void execute_command(custom_info_t *info);

/* my_parser.c */
int is_custom_command(custom_info_t *info, char *path);
char *duplicate_custom_string(char *pathstr, int start, int stop);
char *locate_executable_path(custom_info_t *info, char *pathstr, char *cmd);

/* my_custom_shell.c
int launch_custom_shell(char **);*/

/* my_error_handling.c */
void custom_puts(char *str);
int custom_putchar(char c);
int write_to_fd(char c, int fd);
int write_str_to_fd(char *str, int fd);

/* my_string_operations.c */
int custom_strlen(char *s);
int custom_strcmp(char *s1, char *s2);
char *custom_starts_with(const char *haystack, const char *needle);
char *custom_strcat(char *dest, char *src);

/* my_string_operations_1.c */
char *custom_strcpy(char *dest, char *src);
char *custom_strdup(const char *str);
void custom_puts(char *str);
int custom_putchar(char c);

/* my_exit_handlers.c */
char *custom_strncpy(char *dest, char *src, int n);
char *custom_strncat(char *dest, char *src, int n);
char *custom_strchr(char *s, char c);

/* my_tokenizer.c */
char **split_into_words(char *str, char *delimiters);
char **split_into_words_2(char *str, char delimiter);

/*filer.c*/
int main(int ac, char **av);

/* my_dynamic_memory.c */
char *custom_memset(char *s, char b, unsigned int n);
void custom_ffree(char **pp);
void *custom_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* my_memory_operations.c */
int custom_release_memory(void **ptr)

/* my_numeric_operations.c */
int interactive_custom(custom_info_t *info);
int is_custom_delimiter(char c, char *delim);
int custom_is_alpha(int c);
int custom_atoi(char *s);

/* my_error_handlers.c */
int custom_error_atoi(char *s);
void display_custom_error(custom_info_t *info, char *estr);
int print_custom_decimal(int input, int fd);
char *convert_to_custom_number(long int num, int base, int flags);
void remove_comments(char *buf);

/* my_builtin_functions.c */
int custom_exit(custom_info_t *info);
int custom_cd(custom_info_t *info);
int custom_help(custom_info_t *info);

/* my_builtin_functions_1.c */
int custom_history(custom_info_t *info);
int set_custom_alias(custom_info_t *info, char *str);
int unset_custom_alias(custom_info_t *info, char *str);
int custom_alias(custom_info_t *info);
int print_custom_alias(custom_list_t *node);

/*my_get_input.c */
ssize_t get_input_custom(custom_info_t *info);
ssize_t input_buf_custom(custom_info_t *info, char **buf, size_t *len);
ssize_t custom_getline(custom_info_t *info, char **ptr, size_t *length);
void handle_sigint_custom(int sig_num);

/* my_info_handler.c */
void clear_custom_info(custom_info_t *info);
void set_custom_info(custom_info_t *info, char **av);
void free_custom_info(custom_info_t *info, int all);

/* my_environment.c */
int custom_env(custom_info_t *info);
char *get_custom_env(custom_info_t *info, const char *name);
int set_custom_env(custom_info_t *info);
int unset_custom_env(custom_info_t *info);
int populate_custom_env_list(custom_info_t *info);

/* my_environment_1.c */
char **get_custom_environ(custom_info_t *info);
int unset_custom_env(custom_info_t *info, char *var);
int set_custom_env(custom_info_t *info, char *var, char *value);

/* my_history.c */
char *get_custom_history_file(custom_info_t *info);
int write_custom_history(custom_info_t *info);
int read_custom_history(custom_info_t *info);
int build_custom_history_list(custom_info_t *info, char *buf, int linecount);
int renumber_custom_history(custom_info_t *info);

/* my_linked_lists.c */
custom_list_t *add_custom_node(custom_list_t **head, const char *str, int num);
custom_list_t *add_custom_node_end(custom_list_t **head, const char *str, int num);
size_t print_custom_list_str(const custom_list_t *head);
int delete_custom_node_at_index(custom_list_t **head, unsigned int index);
void custom_free_list(custom_list_t **head_ptr);

/* my_linked_lists_1.c */
size_t custom_list_len(const custom_list_t *h);
char **custom_list_to_strings(custom_list_t *head);
size_t custom_print_list(const custom_list_t *h);
custom_list_t *custom_node_starts_with(custom_list_t *node, char *prefix, char c);
ssize_t custom_get_node_index(custom_list_t *head, custom_list_t *node);

/* my_variable_substitution.c */
int is_custom_chain(custom_info_t *info, char *buf, size_t *p);
void handle_custom_chain(custom_info_t *info, char *buf, size_t *p, size_t i, size_t len);
int custom_alias_substitution(custom_info_t *info);
int custom_variable_substitution(custom_info_t *info);
int custom_string_modification(char **old, char *new);

#endif
