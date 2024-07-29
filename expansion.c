#include "minishell.h"

char *append_char_to_string(char *str, char c)
{
    char *new_str;
    int i;

    i = 0;
    if (str == NULL)
    {
        new_str = (char *)malloc(2);
        new_str[0] = c;
        new_str[1] = '\0';
    }
    else
    {
        new_str = (char *)malloc(ft_strlen(str) + 2);
        while (str[i])
        {
            new_str[i] = str[i];
            i++;
        }
        new_str[i] = c;
        new_str[i + 1] = '\0';
    }
    return new_str;
}

char *append_alnum(char *key, char *str, int *i)
{
    while (str[*i] && is_alnum_or_special(str[*i]))
    {
        key = append_char_to_string(key, str[*i]);
        (*i)++;
    }
    return key;
}

char *expandsion(t_lexer **lexer, char *str, enum token_type type, t_envp *list_envp)
{
    int i;
    int inside_single_quotes;
    int inside_double_quotes;
    char *key;
    char *final_str;

    i = 0;
    inside_single_quotes = 0;
    inside_double_quotes = 0;
    key = NULL;
    final_str = str;
    while (str[i])
    {
        if (str[i] == '"' && inside_double_quotes == 0)
        {
            inside_double_quotes = 1;
        }
        else if (str[i] == '\'' && !inside_double_quotes && inside_single_quotes == 0)
        {
            inside_single_quotes = 1;
            break;
        }

        if (!inside_single_quotes && str[i] == '$' && str[i + 1] == '\0')
        {
            key = append_char_to_string(key, str[i]);
            final_str = replace_env_keys_with_values(final_str, key, list_envp);
            key = NULL;
        }
        else if (!inside_single_quotes && str[i] == '$' && ft_isdigit(str[i + 1]))
        {
            i += 2;
            key = append_alnum(key, str, &i);
            final_str = replace_env_keys_with_values(final_str, key, list_envp);
            key = NULL;
        }
        else if (!inside_single_quotes && str[i] == '$' && ft_isalpha(str[i + 1]))
        {
            i += 1;
            key = append_alnum(key, str, &i);
            final_str = replace_env_keys_with_values(final_str, key, list_envp);
            key = NULL;
        }
        else if (!inside_single_quotes && str[i] == '$' && str[i + 1] == '?')
        {
            i += 2;
            key = append_char_to_string(key, '0');
            final_str = replace_env_keys_with_values(final_str, key, list_envp);
            key = NULL;
        }
        else if (!inside_single_quotes && str[i] == '$' && str[i + 1] == '$')
        {
            i += 2;
            key = append_char_to_string(key, '0');
            final_str = replace_env_keys_with_values(final_str, key, list_envp);
            key = NULL;
        }
        else
            i++;
    }
    return final_str;
}