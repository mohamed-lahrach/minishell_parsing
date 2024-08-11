#include "minishell.h"

int check_for_closed_quotes(char *input)
{
    char *str = input;
    char current_quote = 0;

    while (*str != '\0')
    {
        if (*str == '"' || *str == '\'')
        {
            current_quote = *str;
            str++;

            while (*str != '\0' && *str != current_quote)
                str++;
            if (*str == '\0')
                return 0;
        }
        str++;
    }

    return 1;
}

char *add_spaces_around_special_chars(const char *input)
{
    char *modified_input;
    int i;
    int j;
    int inside_double_quotes;
    int inside_single_quotes;
    
    i = 0;
    j = 0;
    inside_double_quotes = 0;
    inside_single_quotes = 0;
    modified_input = malloc(strlen(input) * 4 + 1);
    if (modified_input == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    while (input[i] != '\0')
    {
        if (input[i] == '"')
        {
            inside_double_quotes = !inside_double_quotes;
            modified_input[j++] = input[i];
        }
        else if (input[i] == '\'')
        {
            inside_single_quotes = !inside_single_quotes;
            modified_input[j++] = input[i];
        }
        else if (!inside_double_quotes && !inside_single_quotes &&
                 (input[i] == '<' || input[i] == '|' || input[i] == '>' || input[i] == '&'))
        {
            if ((input[i] == '&' && input[i + 1] == '&') ||
                (input[i] == '|' && input[i + 1] == '|'))
            {
                free(modified_input);
                return NULL;
            }

            modified_input[j++] = ' ';
            modified_input[j++] = input[i];

            if ((input[i] == '>' && input[i + 1] == '>') ||
                (input[i] == '<' && input[i + 1] == '<'))
            {
                modified_input[j++] = input[++i];
            }

            modified_input[j++] = ' ';
        }
        else
        {
            modified_input[j++] = input[i];
        }
        i++;
    }
    modified_input[j] = '\0';
    return modified_input;
}

size_t strlen_to_char(const char *s, char c)
{
    int len = 0;
    while (s[len] != '\0' && s[len] != c)
        len++;
    return len;
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_string;
	size_t	i;

	sub_string = (char *)malloc(sizeof(*sub_string) * (len + 1));
	if (sub_string == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		sub_string[i] = s[start + i];
		i++;
	}
	sub_string[i] = '\0';
	return (sub_string);
}
void show_lexer(t_lexer *lexer)
{
    t_lexer *current = lexer;
    char *token_type[6] =
        {
            "PIPE",            // = |
            "REDIRECT_OUT",    // = >
            "REDIRECT_IN",     // = <
            "REDIRECT_APPEND", // = >>
            "REDIRECT_INPUT",  //= <<
            "WORD",
        };
    while (current != NULL)
    {
        printf("value: %s , ", current->value);
        printf("type: %s\n", token_type[current->type]);
        
        if (current->prev != NULL)
            printf("prev value: %s\n", current->prev->value);
        else
            printf("prev value: NULL\n");

        if (current->next != NULL)
            printf("next value: %s\n", current->next->value);
        else
            printf("next value: NULL\n");

        printf("\n");
        current = current->next;
    }
}