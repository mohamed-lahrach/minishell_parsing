#include "minishell.h"

void show_command(t_command *command)
{
	t_command *current;
	t_file *file;
	int i;
	int j;

	current = command;
	j = 0;
	char *token_type[6] = {
		"PIPE",			   // = |
		"REDIRECT_OUT",	   // = >
		"REDIRECT_IN",	   // = <
		"REDIRECT_APPEND", // = >>
		"REDIRECT_INPUT",  //= <<
		"WORD",
	};
	char *is_ambiguous[2] = {
		"false",
		"true",
	};
	char *is_quoted[2] = {
		"false",
		"true",
	};
	while (current != NULL)
	{
		i = 0;
		printf("command-%i:\n", j + 1);
		if (current->command_chain != NULL)
			while (current->command_chain[i] != NULL)
			{
				printf("arr[%i]: %s, ", i, current->command_chain[i]);
				i++;
			}
		printf("\n");
		file = current->file;
		while (file != NULL)
		{
			printf("filename : `%s` filetype : `%s`  is_ambiguous `%s` is_quoted `%s`\n", file->file_name,
				   token_type[file->file_type], is_ambiguous[file->is_ambiguous], is_quoted[file->is_quoted]);
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
		expansion_phase(lexer, list_envp);
		// if (syntax_error(*lexer))
		// {
		// 	free_list(lexer);
		// 	free(input);
		// 	continue ;
		// }
		command = parser_phase(*lexer);
		// for debugging
		show_command(command);
		free_lexer_list(lexer);
		free(input);
	}
}
t_envp *create_env_node(const char *key, const char *value)
{
	t_envp *new_node = (t_envp *)malloc(sizeof(t_envp));
	if (!new_node)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	new_node->key = strdup(key);
	new_node->value = strdup(value);
	new_node->next = NULL;
	return new_node;
}

// Function to add a new node to the environment list
void add_env_node(t_envp **env, const char *key, const char *value)
{
	t_envp *new_node = create_env_node(key, value);
	t_envp *current = *env;

	if (!current)
	{
		*env = new_node;
		return;
	}
	while (current->next)
		current = current->next;
	current->next = new_node;
}

void show_env(t_envp *env)
{
	t_envp *current = env;
	while (current)
	{
		printf("key: %s, value: %s\n", current->key, current->value);
		current = current->next;
	}
}
int main(int argc, char **argv, char **envp)
{
	t_lexer *lexer;
	t_envp *list_envp;

	lexer = NULL;
	if (argc != 1)
	{
		printf("This program does not accept arguments\n");
		exit(0);
	}
	list_envp = create_environment_node(envp);
	add_env_node(&list_envp, "cmd1", "ls -l -e -m -a -t");
	add_env_node(&list_envp, "cmd2", "wc -l -c -w -m -e");
	add_env_node(&list_envp, "cmd3", "echo hello world");
	show_env(list_envp);
	minishell_process(&lexer, list_envp);
	return (0);
}
