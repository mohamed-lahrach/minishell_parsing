#include "minishell.h"

char	*ft_getenv(t_envp *env, char *key)
{
	t_envp	*current;

    current = env;
    while (current != NULL)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}