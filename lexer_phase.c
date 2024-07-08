#include "minishell.h"

int get_start_by_str_address(char *input, char *str)
{
    int start = 0;
    while (input != str)
    {
        input++;
        start++;
    }
    return start;
}
void lexer_phase(t_lexer **lexer, char *input)
{
    char *str = input;
    char current_word[256];
    int word_index = 0;
    char token_str[256];
    enum token_type type;
    int inside_quotes = 0;
    char *my_word;
    int start = 0;
    int i = 0;

    while (*str != '\0')
    {
        if (*str == '"' && inside_quotes == 0)
        {
            inside_quotes = 1;
            current_word[word_index++] = *str;
        }
        else if (*str == '"' && inside_quotes == 1)
        {
            inside_quotes = 0;
            current_word[word_index++] = *str;
        }
        else if (*str == '\'' && inside_quotes == 0)
        {
            inside_quotes = 2;
            current_word[word_index++] = *str;
        }
        else if (*str == '\'' && inside_quotes == 2)
        {
            inside_quotes = 0;
            current_word[word_index++] = *str;
        }
        else if (inside_quotes)
            current_word[word_index++] = *str;
        else
        {
            if (*str == ' ' || *str == '\t')
            {
                if (word_index > 0)
                {
                    current_word[word_index] = '\0';
                    my_word = ft_substr(input, start, word_index);
                    append_lexer_node(lexer, create_lexer_node(current_word, WORD));
                    word_index = 0;
                    start = i + 1;
                }
            }
            else if (*str == '<' || *str == '|' || *str == '>')
            {
                if (word_index > 0)
                {
                    current_word[word_index] = '\0';
                    append_lexer_node(lexer, create_lexer_node(current_word, WORD));
                    word_index = 0;
                }
                if ((*str == '>' && *(str + 1) == '>') || (*str == '<' && *(str + 1) == '<'))
                {
                    type = (*str == '>') ? REDIRECT_APPEND : REDIRECT_INPUT;
                    token_str[0] = *str;
                    token_str[1] = *(str + 1);
                    token_str[2] = '\0';
                    str++;
                }
                else if (*str == '<' || *str == '>')
                {
                    type = (*str == '>') ? REDIRECT_OUT : REDIRECT_IN;
                    token_str[0] = *str;
                    token_str[1] = '\0';
                }
                else if (*str == '|')
                {
                    type = PIPE;
                    token_str[0] = *str;
                    token_str[1] = '\0';
                }
                append_lexer_node(lexer, create_lexer_node(token_str, type));
                start = i + 1;
            }
            else
                current_word[word_index++] = *str;
        }
        str++;
        i++;
        //start = get_start_by_str_address(input, str) + 1;
    }

    if (word_index > 0)
    {
        current_word[word_index] = '\0';
        append_lexer_node(lexer, create_lexer_node(current_word, WORD));
    }
}
