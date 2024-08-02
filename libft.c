#include "minishell.h"

size_t ft_strlen(const char *s)
{
    int len;

    len = 0;
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

t_envp *create_envp_node(char *line_envp)
{
    t_envp *new_node = (t_envp *)malloc(sizeof(t_envp));
    char *key;
    char *value;

    if (!new_node)
        return NULL;
    key = ft_substr(line_envp, 0, strlen_to_char(line_envp, '='));
    value = ft_substr(line_envp, ft_strlen(key) + 1, ft_strlen(line_envp + ft_strlen(key) + 1));
    if (!key || !value)
    {
        free(new_node);
        return NULL;
    }
    new_node->key = key;
    new_node->value = value;
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
void append_envp_node(t_envp **envir, t_envp *new_node)
{
    if (*envir == NULL)
    {
        *envir = new_node;
    }
    else
    {
        t_envp *current = *envir;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

void free_list(t_lexer **lexer)
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

t_envp *create_environment_node(char **envp)
{
    t_envp *list;
    int i;

    list = NULL;
    i = 0;
    while (envp[i])
    {
        append_envp_node(&list, create_envp_node(envp[i]));
        i++;
    }

    return list;
}

int ft_strcmp(const char *s1, const char *s2)
{

    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int ft_isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return (1);
    else
        return (0);
}

int ft_isalnum(int c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        return (1);
    else
        return (0);
}

int ft_isalpha(int c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return (1);
    else
        return (0);
}
int is_alnum_or_underscore(char c)
{
    return ft_isalnum(c) || c == '_';
}