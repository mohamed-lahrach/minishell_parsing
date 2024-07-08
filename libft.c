#include "minishell.h"

size_t ft_strlen(const char *s)
{
    int len = 0;
    while (s[len] != '\0')
        len++;
    return len;
}

char *ft_strdup(const char *str)
{
    int idx;
    char *dest = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
    if (dest == NULL)
        return NULL;
    for (idx = 0; str[idx]; idx++)
    {
        dest[idx] = str[idx];
    }
    dest[idx] = '\0';
    return dest;
}

t_lexer *create_lexer_node(char *value, enum token_type type)
{
    t_lexer *new_node = (t_lexer *)malloc(sizeof(t_lexer));
    if (!new_node)
        return NULL;
    new_node->value = ft_strdup(value);
    new_node->type = type;
    new_node->next = NULL;
    return new_node;
}

void append_lexer_node(t_lexer **lexer, t_lexer *new_node)
{
    if (*lexer == NULL)
    {
        *lexer = new_node;
    }
    else
    {
        t_lexer *current = *lexer;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

void free_lexer(t_lexer **lexer)
{
    t_lexer *current = *lexer;
    t_lexer *next;
    while (current != NULL)
    {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    *lexer = NULL;
}
