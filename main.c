#include "minishell.h"

int main(int argc, char **argv)
{
    t_lexer *lexer = NULL;
    char *input;
    if (argc != 1)
	{
		printf("This program does not accept arguments\n");
		exit(0);
	}
    while (1) 
    {
        input = readline("minishell$ ");
        if (input == NULL || strcmp(input, "exit") == 0)
        {
            if (input == NULL)
                printf("No input provided\n");
            else
                printf("Exiting...\n");
            break;
        }

        add_history(input);

        if (check_for_closed_quotes(input) == 0)
        {
            printf("Syntax Error: quotes are not closed\n");
            free(input);
            continue;
        }
        char *modified_input = add_spaces_around_special_chars(input);
        if (modified_input == NULL)
        {
            printf("Syntax Error\n");
            free(input);
            continue;
        }

        lexer_phase(&lexer, modified_input);

        t_lexer *current = lexer;
        const char *token_names[] = {
            "PIPE",
            "REDIRECT_OUT",
            "REDIRECT_IN",
            "REDIRECT_APPEND",
            "REDIRECT_INPUT",
            "WORD",
            "WHITESPACE",
        };
        printf("%s\n", modified_input);
        while (current)
        {
            printf("Type: {%s}, Value: `%s`\n--------------------------------------\n", token_names[current->type], current->value);
            current = current->next;
        }

        free_lexer(&lexer);
        free(input);
        free(modified_input);
    }
    //parser_phase(lexer);
    return 0;
}
