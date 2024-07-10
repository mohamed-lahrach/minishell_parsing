#include "minishell.h"

void lexer_phase(t_lexer **lexer, char *input)
{
    char *str;
    int inside_quotes;
    int word_index;
    int start;
    int i;
    char *my_word;
    enum token_type type;

    str = input;
    inside_quotes = 0;
    word_index = 0;
    start = 0;
    i = 0;
    while (*str != '\0')
    {
        if (*str == '"' && inside_quotes == 0)
        {
            inside_quotes = 1;
            word_index++;
        }
        else if (*str == '"' && inside_quotes == 1)
        {
            inside_quotes = 0;
            word_index++;
        }
        else if (*str == '\'' && inside_quotes == 0)
        {
            inside_quotes = 2;
            word_index++;
        }
        else if (*str == '\'' && inside_quotes == 2)
        {
            inside_quotes = 0;
            word_index++;
        }
        else if (inside_quotes)
            word_index++;
        else
        {
            if (*str == ' ' || *str == '\t')
            {
                if (word_index > 0)
                {
                    my_word = ft_substr(input, start, word_index);
                    append_lexer_node(lexer, create_lexer_node(my_word, WORD));
                    word_index = 0;
                }
                start = i + 1;
            }
            else if (*str == '<' || *str == '|' || *str == '>')
            {
                if (word_index > 0)
                {
                    my_word = ft_substr(input, start, word_index);
                    append_lexer_node(lexer, create_lexer_node(my_word, WORD));
                    word_index = 0;
                }
                if ((*str == '>' && *(str + 1) == '>') || (*str == '<' && *(str + 1) == '<'))
                {
                    type = (*str == '>') ? REDIRECT_APPEND : REDIRECT_INPUT;
                    my_word = ft_substr(str, 0, 2);
                    str++;
                    i++;
                }
                else if (*str == '<' || *str == '>')
                {
                    type = (*str == '>') ? REDIRECT_OUT : REDIRECT_IN;
                    my_word = ft_substr(str, 0, 1);
                }
                else if (*str == '|')
                {
                    type = PIPE;
                    my_word = ft_substr(str, 0, 1);
                }
                append_lexer_node(lexer, create_lexer_node(my_word, type));
            }
            else
                word_index++;
        }
        str++;
        i++;
    }

    if (word_index > 0)
    {
        my_word = ft_substr(input, start, word_index);
        append_lexer_node(lexer, create_lexer_node(my_word, WORD));
    }
}
