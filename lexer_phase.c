#include "minishell.h"

void show_envp(t_envp *envp)
{
    t_envp *tmp;

    tmp = envp;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, "HOME") == 0)
        {
            printf("key: %s : ", tmp->key);
            printf("value: %s\n", tmp->value);
        }
        tmp = tmp->next;
    }
}

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

int is_alnum_or_special(char c)
{
    return ft_isalnum(c) || c == '_';
}
char *append_alnum_chars(char *expanded_str, const char *value, int *i)
{
    while (value[*i] && is_alnum_or_special(value[*i]))
    {
        expanded_str = append_char_to_string(expanded_str, value[*i]);
        (*i)++;
    }
    return expanded_str;
}

char *expand_env_vars(char *value, char *expanded_str, char *get_env)
{
    int i;
    int k;
    int j;
    size_t final_str_len;
    char *final_str;
    int len_expaded_str;

    len_expaded_str = ft_strlen(expanded_str);
    final_str_len = ft_strlen(value) + ft_strlen(get_env) - len_expaded_str + 1; // Allocate enough space for the final string
    final_str = (char *)malloc(final_str_len * sizeof(char));
    if (final_str == NULL)
    {
        free(final_str);
        return NULL;
    }

    i = 0;
    k = 0;
    while (value[i])
    {
        if (value[i] != '$')
        {
            final_str[k] = value[i];
            i++;
            k++;
        }
        else
        {
            j = 0;
            while (get_env[j])
            {
                final_str[k] = get_env[j];
                j++;
                k++;
            }
            i += 1 + len_expaded_str; // Skip the '$' character
        }
    }
    final_str[k] = '\0'; // Null-terminate the final string
    return final_str;
}

void expanding(char *value, char *expanded_str, t_envp *list_envp)
{
    char *get_env;
    char *final_str;

    get_env = getenv(expanded_str);
    final_str = expand_env_vars(value, expanded_str, get_env);
    printf("final_str: %s\n", final_str);
    printf("get_env: %s\n", get_env);
    printf("value: %s\n", value);
    printf("expanded_str: %s\n", expanded_str);

    free(final_str); // Free the allocated memory
}
char *expand_word(t_lexer **lexer, char *value, enum token_type type, t_envp *list_envp)
{
    int i;
    int inside_single_quotes;
    int inside_double_quotes;
    char *expanded_str;

    i = 0;
    inside_single_quotes = 0;
    inside_double_quotes = 0;
    expanded_str = NULL;

    while (value[i])
    {
        if (value[i] == '"' && inside_double_quotes == 0)
        {
            inside_double_quotes = 1;
        }
        else if (value[i] == '\'' && !inside_double_quotes && inside_single_quotes == 0)
        {
            inside_single_quotes = 1;
            break;
        }

        if (!inside_single_quotes && value[i] == '$' && value[i + 1] == '\0')
        {
            expanded_str = append_char_to_string(expanded_str, value[i]);
        }
        else if (!inside_single_quotes && value[i] == '$' && ft_isdigit(value[i + 1]))
        {
            i += 2;
            expanded_str = append_alnum_chars(expanded_str, value, &i);
            expanding(value, expanded_str, list_envp);
        }
        else if (!inside_single_quotes && value[i] == '$' && ft_isalpha(value[i + 1]))
        {
            i += 1;
            expanded_str = append_alnum_chars(expanded_str, value, &i);
            expanding(value, expanded_str, list_envp);
        }
        else if (!inside_single_quotes && value[i] == '$' && value[i + 1] == '?')
        {
            i += 2;
            expanded_str = append_char_to_string(expanded_str, '0');
        }
        else if (!inside_single_quotes && value[i] == '$' && value[i + 1] == '$')
        {
            i += 2;
            expanded_str = append_char_to_string(expanded_str, '0');
        }
        else
            i++;
    }
    return expanded_str;
}

void lexer_phase(t_lexer **lexer, char *input, t_envp *list_envp)
{
    int inside_quotes;
    int current_word_len;
    int start;
    int i;
    char *current_word;
    enum token_type type;

    inside_quotes = 0;
    current_word_len = 0;
    start = 0;
    i = 0;
    while (input[i] != '\0')
    {
        if (input[i] == '"' && inside_quotes == 0)
        {
            inside_quotes = 1;
            current_word_len++;
        }
        else if (input[i] == '"' && inside_quotes == 1)
        {
            inside_quotes = 0;
            current_word_len++;
        }
        else if (input[i] == '\'' && inside_quotes == 0)
        {
            inside_quotes = 2;
            current_word_len++;
        }
        else if (input[i] == '\'' && inside_quotes == 2)
        {
            inside_quotes = 0;
            current_word_len++;
        }
        else if (inside_quotes)
            current_word_len++;
        else
        {
            if (input[i] == ' ' || input[i] == '\t')
            {
                if (current_word_len > 0)
                {
                    current_word = ft_substr(input, start, current_word_len);
                    append_node(lexer, create_node(current_word, WORD));
                    expand_word(lexer, current_word, WORD, list_envp);
                    current_word_len = 0;
                }
                start = i + 1;
            }
            else if (input[i] == '<' || input[i] == '|' || input[i] == '>')
            {
                if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
                {
                    type = (input[i] == '>') ? REDIRECT_APPEND : REDIRECT_INPUT;
                    current_word = ft_substr(input + i, 0, 2);
                    i++;
                }
                else if (input[i] == '<' || input[i] == '>')
                {
                    type = (input[i] == '>') ? REDIRECT_OUT : REDIRECT_IN;
                    current_word = ft_substr(input + i, 0, 1);
                }
                else if (input[i] == '|')
                {
                    type = PIPE;
                    current_word = ft_substr(input + i, 0, 1);
                }
                append_node(lexer, create_node(current_word, type));
                // expanding(lexer, current_word, WORD, list_envp);
            }
            else
                current_word_len++;
        }
        i++;
    }

    if (current_word_len > 0)
    {
        current_word = ft_substr(input, start, current_word_len);
        append_node(lexer, create_node(current_word, WORD));
        expand_word(lexer, current_word, WORD, list_envp);
    }
}
