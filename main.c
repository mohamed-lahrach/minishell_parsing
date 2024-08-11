#include "minishell.h"

char **add_command_to_array(char **cmd_chain, char *cmd)
{
    int i;
    char **new_cmd_chain;

    i = 0;
    if (cmd_chain == NULL)
    {
        new_cmd_chain = (char **)malloc(sizeof(char *) * 2);
        new_cmd_chain[0] = ft_strdup(cmd);
        new_cmd_chain[1] = NULL;
    }
    else
    {
        while (cmd_chain[i] != NULL)
            i++;
        new_cmd_chain = (char **)malloc(sizeof(char *) * (i + 2));
        i = 0;
        while (cmd_chain[i] != NULL)
        {
            new_cmd_chain[i] = ft_strdup(cmd_chain[i]);
            i++;
        }
        new_cmd_chain[i] = ft_strdup(cmd);
        new_cmd_chain[i + 1] = NULL;
    }
    return new_cmd_chain;
}
t_file *add_file_to_list(t_file **file, char *file_name, enum token_type file_type)
{
    t_file *new_file;
    t_file *current;

    new_file = (t_file *)malloc(sizeof(t_file));
    if (new_file == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    new_file->file_name = ft_strdup(file_name);
    new_file->file_type = file_type;
    new_file->next = NULL;
    if (*file == NULL)
    {
        *file = new_file;
    }
    else
    {
        current = *file;
        while (current->next != NULL)
            current = current->next;
        current->next = new_file;
    }
    return *file;
}

t_command *add_command_to_list(t_command **cmds, char ***cmd_chain, t_file **file)
{
    t_command *new_command;
    t_command *current;
    t_file *current_file;
    int i;

    new_command = (t_command *)malloc(sizeof(t_command));
    if (new_command == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    new_command->command_chain = *cmd_chain;
    new_command->file = *file;
    new_command->next = NULL;
    if (*cmds == NULL)
    {
        *cmds = new_command;
    }
    else
    {
        current = *cmds;
        while (current->next != NULL)
            current = current->next;
        current->next = new_command;
    }
    return *cmds;
}
t_command *parser_phase(t_lexer *lexer)
{
    t_command *cmds;
    t_file *file;
    char **cmd_chain;

    cmds = NULL;
    file = NULL;
    cmd_chain = NULL;
    while (lexer)
    {
        if (lexer->type == WORD && lexer->prev == NULL)
            cmd_chain = add_command_to_array(cmd_chain, lexer->value);
        else if (lexer->type == WORD && lexer->prev->type == REDIRECT_IN)
            file = add_file_to_list(&file, lexer->value, REDIRECT_IN);
        else if (lexer->type == WORD && lexer->prev->type == REDIRECT_OUT)
            file = add_file_to_list(&file, lexer->value, REDIRECT_OUT);
        else if (lexer->type == WORD && lexer->prev->type == REDIRECT_APPEND)
            file = add_file_to_list(&file, lexer->value, REDIRECT_APPEND);
        else if (lexer->type == WORD && lexer->prev->type == REDIRECT_INPUT)
            file = add_file_to_list(&file, lexer->value, REDIRECT_INPUT);
        else if (lexer->type == WORD)
            cmd_chain = add_command_to_array(cmd_chain, lexer->value);
        else if (lexer->type == PIPE)
        {
            cmds = add_command_to_list(&cmds, &cmd_chain, &file);
            cmd_chain = NULL;
            file = NULL;
        }
        lexer = lexer->next;
    }
    if (cmd_chain != NULL)
        cmds = add_command_to_list(&cmds, &cmd_chain, &file);
    return cmds;
}
void show_command(t_command *command)
{
    t_command *current = command;
    t_file *file;
    int i;
    int j;

    j = 0;

    while (current != NULL)
    {
        i = 0;
        printf("command-%i: ", j+1);
        while (current->command_chain[i] != NULL)
        {
            printf("%s ", current->command_chain[i]);
            i++;
        }
        printf("\n");
        char *token_type[6] =
            {
                "PIPE",            // = |
                "REDIRECT_OUT",    // = >
                "REDIRECT_IN",     // = <
                "REDIRECT_APPEND", // = >>
                "REDIRECT_INPUT",  //= <<
                "WORD",
            };
        file = current->file;
        while (file != NULL)
        {
            printf("filename : `%s` filetype : `%s`", file->file_name, token_type[file->file_type]);
            file = file->next;
        }
        current = current->next;
        printf("\n");
        j++;
    }
}
void minishell_process(t_lexer **lexer, t_envp *list_envp)
{
    char *input;
    t_command *command;

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
        // show_lexer(*lexer);
        command = parser_phase(*lexer);
        // for debugging
        show_command(command);
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
