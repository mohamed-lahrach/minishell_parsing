#include "minishell.h"
int get_target_dollar(char *str, char *key)
{
    int i;
    int j;
    int k;
    int len_expanded_str;

    i = 0;
    j = 0;
    k = 0;
    len_expanded_str = ft_strlen(key);
    while (str[i])
    {
        if (str[i] == '$')
        {
            j = i + 1;
            while (str[j] == key[k])
            {
                j++;
                k++;
            }
            if (k == len_expanded_str)
                return i;
        }
        i++;
    }
    return -1;
}

char *replace_env_keys_with_values(char *str, char *key, t_envp *list_envp)
{
    char *value;
    int i;
    int k;
    int j;
    size_t final_str_len;
    char *final_str;
    int len_key;
    int target_dollar;

    value = getenv(key);
    len_key = ft_strlen(key);
    final_str_len = ft_strlen(str) + ft_strlen(value) - len_key + 1;
    final_str = (char *)malloc(final_str_len * sizeof(char));
    if (final_str == NULL)
    {
        free(final_str);
        return NULL;
    }

    i = 0;
    k = 0;
    target_dollar = get_target_dollar(str, key);
    while (str[i])
    {
        if (i != target_dollar)
        {
            final_str[k] = str[i];
            i++;
            k++;
        }
        else
        {
            j = 0;
            while (value[j])
            {
                final_str[k] = value[j];
                j++;
                k++;
            }
            i += 1 + len_key;
            break;
        }
    }
    while (str[i])
    {
        final_str[k] = str[i];
        i++;
        k++;
    }

    final_str[k] = '\0';
    str = final_str;
    return final_str;
}