#include "minishell.h"

void lexer_phase(t_lexer **lexer, char *input, t_envp *list_envp)
{
    int inside_quotes;
    int current_word_len;
    int start;
    int i;
    char *current_word;
    enum token_type type;
    char *expanded_word;

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
                    expanded_word = expansion(lexer, current_word, WORD, list_envp);
                    remove_quotes(&expanded_word);
                    append_lexer_node(lexer, create_lexer_node(expanded_word, WORD));
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
                append_lexer_node(lexer, create_lexer_node(current_word, type));
            }
            else
                current_word_len++;
        }
        i++;
    }

    if (current_word_len > 0)
    {
        current_word = ft_substr(input, start, current_word_len);
        expanded_word = expansion(lexer, current_word, WORD, list_envp);
        remove_quotes(&expanded_word);
        append_lexer_node(lexer, create_lexer_node(expanded_word, WORD));
    }
}
