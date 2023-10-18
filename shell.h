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
int custom_shell(custom_info_t *, char **);
int identify_builtin_command(custom_info_t *);
void locate_command(custom_info_t *);
void execute_command(custom_info_t *);

/* my_parser.c */
int is_custom_command(custom_info_t *, char *);
char *duplicate_custom_string(char *, int, int);
char *locate_executable_path(custom_info_t *, char *, char *);

/* my_custom_shell.c */
int launch_custom_shell(char **);

/* my_error_handling.c */
void custom_puts(char *);
int custom_putchar(char);
int write_to_fd(char c, int fd);
int write_str_to_fd(char *str, int fd);

/* my_string_operations.c */
int custom_strlen(char *);
int compare_custom_strings(char *, char *);
char *start_with(const char *, const char *);
char *custom_strcat(char *, char *);

/* my_string_operations_1.c */
char *copy_custom_string(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* my_exit_handlers.c */
char *custom_strncpy(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* my_tokenizer.c */
char **split_into_words(char *, char *);
char **split_into_words_2(char *, char);

/* my_dynamic_memory.c */
char *custom_memset(char *, char, unsigned int);
void custom_free(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* my_memory_operations.c */
int release_memory(void **);

/* my_numeric_operations.c */
int interactive_custom(custom_info_t *);
int is_custom_delimiter(char, char *);
int is_custom_alpha(int);
int custom_atoi(char *);

/* my_error_handlers.c */
int custom_error_atoi(char *);
void display_custom_error(custom_info_t *, char *);
int print_custom_decimal(int, int);
char *convert_to_custom_number(long int, int, int);
void remove_comments(char *);

/* my_builtin_functions.c */
int custom_exit(custom_info_t *);
int custom_cd(custom_info_t *);
int custom_help(custom_info_t *);

/* my_builtin_functions_1.c */
int custom_history(custom_info_t *);
int custom_alias(custom_info_t *);

/*my_get_input.c */
ssize_t get_input_custom(custom_info_t *);
int custom_getline(custom_info_t *, char **, size_t *);
void handle_sigint_custom(int);

/* my_info_handler.c */
void clear_custom_info(custom_info_t *);
void set_custom_info(custom_info_t *, char **);
void free_custom_info(custom_info_t *, int);

/* my_environment.c */
char *get_custom_env(custom_info_t *, const char *);
int custom_env(custom_info_t *);
int set_custom_env(custom_info_t *);
int unset_custom_env(custom_info_t *);
int populate_env_list(custom_info_t *);

/* my_environment_1.c */
char **get_custom_environment(custom_info_t *);
int unset_custom_env(custom_info_t *, char *);
int set_custom_env(custom_info_t *, char *, char *);

/* my_history.c */
char *get_custom_history_file(custom_info_t *info);
int write_custom_history(custom_info_t *info);
int read_custom_history(custom_info_t *info);
int build_custom_history_list(custom_info_t *info, char *buf, int linecount);
int renumber_custom_history(custom_info_t *info);

/* my_linked_lists.c */
custom_list_t *add_custom_node(custom_list_t **, const char *, int);
custom_list_t *add_custom_node_end(custom_list_t **, const char *, int);
size_t print_custom_list_str(const custom_list_t *);
int delete_custom_node_at_index(custom_list_t **, unsigned int);
void custom_free_list(custom_list_t **);

/* my_linked_lists_1.c */
size_t custom_list_length(const custom_list_t *);
char **custom_list_to_strings(custom_list_t *);
size_t custom_list_print(const custom_list_t *);
custom_list_t *custom_node_starts_with(custom_list_t *, char *, char);
ssize_t get_custom_node_index(custom_list_t *, custom_list_t *);

/* my_variable_substitution.c */
int is_custom_chain(custom_info_t *, char *, size_t *);
void handle_custom_chain(custom_info_t *, char *, size_t *, size_t, size_t);
int custom_alias_substitution(custom_info_t *);
int custom_variable_substitution(custom_info_t *);
int custom_string_modification(char **, char *);

#endif
