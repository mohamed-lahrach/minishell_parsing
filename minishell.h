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

typedef struct parser
{
    t_lexer *lexer;
    struct parser *next;
} t_parser;


int check_for_closed_quotes(char *input);
char *add_spaces_around_special_chars(const char *input);
void lexer_phase(t_lexer **lexer, char *input);
size_t ft_strlen(const char *s);
char *ft_strdup(const char *str);
t_lexer *create_lexer_node(char *value, enum token_type type);
void append_lexer_node(t_lexer **lexer, t_lexer *new_node);
void free_lexer(t_lexer **lexer);
char *ft_substr(char const *s, unsigned int start, size_t len);

#endif