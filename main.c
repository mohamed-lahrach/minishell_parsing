#include "minishell.h"

int syntax_error(t_lexer *lexer)
{
    t_lexer *current = lexer;
    while (current)
    {
        if (current->type == PIPE)
        {
            if (current->next == NULL || current->next->type != WORD)
            {
                printf("bash: syntax error near unexpected token `newline' pipe at the end or (pipe followed sepecial char.)\n");
                return 1;
            }
        }
        else if (current->type == REDIRECT_OUT)
        {
            if (current->next == NULL || current->next->type != WORD)
            {
                printf("bash: syntax error near unexpected token `newline' redirect out at the end or (redirect out followed sepecial char.)\n");
                return 1;
            }
        }
        else if (current->type == REDIRECT_IN)
        {
            if (current->next == NULL || current->next->type != WORD)
            {
                printf("bash: syntax error near unexpected token `newline' redirect in at the end or (redirect in followed sepecial char.)\n");
                return 1;
            }
        }
        else if (current->type == REDIRECT_APPEND)
        {
            if (current->next == NULL || current->next->type != WORD)
            {
                printf("bash: syntax error near unexpected token `newline' redirect append at the end or (redirect append followed sepecial char.)\n");
                return 1;
            }
        }
        else if (current->type == REDIRECT_INPUT)
        {
            if (current->next == NULL || current->next->type != WORD)
            {
                printf("bash: syntax error near unexpected token `newline' redirect input at the end or (redirect input followed sepecial char.)\n");
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}

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
        input = add_spaces_around_special_chars(input);
        if (input == NULL)
        {
            printf("Syntax Error\n");
            free(input);
            continue;
        }

        lexer_phase(&lexer, input);
        if (syntax_error(lexer))
        {
            free_lexer(&lexer);
            free(input);
            continue;
        }
        

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
        printf("%s\n", input);
        while (current)
        {
            printf("Type: {%s}, Value: `%s`\n--------------------------------------\n", token_names[current->type], current->value);
            current = current->next;
        }

        free_lexer(&lexer);
        free(input);
    }
    //parser_phase(lexer);
    return 0;
}
