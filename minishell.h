#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
enum token_type
{
	PIPE,			 // = |
	REDIRECT_OUT,	 // = >
	REDIRECT_IN,	 // = <
	REDIRECT_APPEND, // = >>
	REDIRECT_INPUT,	 //= <<
	WORD,
};
typedef struct lexer
{
	char *value;
	enum token_type type;
	struct lexer *prev;
	struct lexer *next;
} t_lexer;
typedef struct envp
{
	char *key;
	char *value;
	struct envp *next;
} t_envp;

typedef struct t_file
{
	char *file_name;
	enum token_type file_type;
	bool is_quoted;
	bool is_ambiguous;
	struct t_file *next;
} t_file;

typedef struct command
{
	char **command_chain;
	t_file *file;
	struct command *next;
} t_command;

// expansion functions
char *append_char_to_string(char *str, char c);
char *append_alnum(char *key, char *value, int *i);
char *replace_env_keys_with_values(char *str, char *key,
								   t_envp *list_envp);
int get_target_dollar(char *str, char *key);
void remove_quotes(char **str);
void expansion_phase(t_lexer **lexer, t_envp *list_envp);

// string manipulation functions
int is_alnum_or_underscore(char c);
int ft_isdigit(int c);
int ft_isalpha(int c);
int ft_isalnum(int c);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strdup(const char *str);
size_t ft_strlen(const char *s);
size_t strlen_to_char(const char *s, char c);
char *ft_substr(char const *s, unsigned int start, size_t len);
char *add_spaces_around_special_chars(const char *input);
int check_for_closed_quotes(char *input);
char **ft_split_pro_max(char *s);
int count_words(char *s);
void process_command_chain_and_files(t_command *command);


// environment functions
t_envp *create_environment_node(char **envp);
t_envp *create_environment_node(char **envp);
t_envp *create_envp_node(char *value);
void append_envp_node(t_envp **envir, t_envp *new_node);
char *ft_getenv(t_envp *env, char *key);

// lexer functions
void lexer_phase(t_lexer **lexer, char *input,
				 t_envp *list_envp);
t_lexer *create_lexer_node(char *value, enum token_type type);
void append_lexer_node(t_lexer **lexer, t_lexer *new_node);
void show_lexer(t_lexer *lexer);
t_lexer *get_last_node(t_lexer *head);

// parser functions
t_command *parser_phase(t_lexer *lexer);
t_command *add_command_to_list(t_command **list_of_commands,
							   char **cmd_chain, t_file **file);
t_file *add_file_to_list(t_file **file, char *file_name,
						 enum token_type file_type);
char **add_cmd_chain_to_list(char **cmd_chain, char *value);

// general functions
void minishell_process(t_lexer **lexer, t_envp *list_envp);
int syntax_error(t_lexer *lexer);
void free_lexer_list(t_lexer **lexer);

#endif