#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
enum token_type
{
    PIPE,            // = |
    REDIRECT_OUT,    // = >
    REDIRECT_IN,     // = <
    REDIRECT_APPEND, // = >>
    REDIRECT_INPUT,  //= <<
    WORD,
};
typedef struct lexer
{
    char *value;
    enum token_type type;
    struct lexer *next;
} t_lexer;
typedef struct envp
{
    char *key;
    char *value;
    struct envp *next;
} t_envp;


int	ft_strcmp(const char *s1, const char *s2);
void show_lexer(t_lexer *lexer);
t_envp *create_environment_node(char **envp);
void minishell_process(t_lexer **lexer, t_envp *list_envp);
size_t strlen_to_char(const char *s, char c);
t_envp *create_envp_node(char *value);
void append_envp_node(t_envp **envir, t_envp *new_node);
t_envp *create_environment_node(char **envp);
int syntax_error(t_lexer *lexer);
int check_for_closed_quotes(char *input);
char *add_spaces_around_special_chars(const char *input);
void lexer_phase(t_lexer **lexer, char *input, t_envp *list_envp);
size_t ft_strlen(const char *s);
char *ft_strdup(const char *str);
t_lexer *create_node(char *value, enum token_type type);
void append_node(t_lexer **lexer, t_lexer *new_node);
void free_list(t_lexer **lexer);
char *ft_substr(char const *s, unsigned int start, size_t len);

#endif