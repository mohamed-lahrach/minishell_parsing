#include "minishell.h"

void expand(t_lexer **lexer, t_envp *list_envp)
{
    printf("expand\n");
}

void minishell_process(t_lexer **lexer, t_envp *list_envp)
{
    char *input;

    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
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
        lexer_phase(lexer, input, list_envp);

        if (syntax_error(*lexer))
        {
            free_list(lexer);
            free(input);
            continue;
        }
        // for debugging
        show_lexer(*lexer);
        free_list(lexer);
        free(input);
    }
}
int main(int argc, char **argv, char **envp)
{
    t_lexer *lexer = NULL;
    t_envp *list_envp;
    if (argc != 1)
    {
        printf("This program does not accept arguments\n");
        exit(0);
    }
    list_envp = create_environment_node(envp);
    minishell_process(&lexer, list_envp);
    return 0;
}
