#include "minishell.h"

char **add_cmd_chain_to_list(char **cmd_chain, char *cmd)
{
	int i;
	int j;
	char **new_cmd_chain;
	char **tmp;
	int cout_words;

	cout_words = 0;
	i = 0;
	tmp = ft_split_pro_max(cmd);
	while (tmp[cout_words] != NULL)
		cout_words++;
	if (cmd_chain == NULL)
	{
		new_cmd_chain = (char **)malloc(sizeof(char *) * (cout_words + 1));
		while (i < cout_words)
		{
			new_cmd_chain[i] = ft_strdup(tmp[i]);
			i++;
		}
		new_cmd_chain[i] = NULL;
	}
	else
	{
		while (cmd_chain[i] != NULL)
			i++;
		new_cmd_chain = (char **)malloc(sizeof(char *) * (i + cout_words + 1));
		i = 0;
		j = 0;
		while (cmd_chain[i] != NULL)
		{
			new_cmd_chain[i] = ft_strdup(cmd_chain[i]);
			i++;
		}
		while (j < cout_words)
		{
			new_cmd_chain[i] = ft_strdup(tmp[j]);
			i++;
			j++;
		}
		new_cmd_chain[i] = NULL;
	}
	return (new_cmd_chain);
}
t_file *add_file_to_list(t_file **file, char *file_name,
						 enum token_type file_type)
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
	if(file_type == REDIRECT_INPUT && (strchr(file_name, '\'') || strchr(file_name, '"')))
		new_file->is_quoted = true;
	else
		new_file->is_quoted = false;
	if (count_words(file_name) > 1)
		new_file->is_ambiguous = true;
	else
		new_file->is_ambiguous = false;
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
	return (*file);
}

t_command *add_command_to_list(t_command **list_of_commands, char **cmd_chain,
							   t_file **file)
{
	t_command *new_command;
	t_command *current;

	new_command = (t_command *)malloc(sizeof(t_command));
	if (new_command == NULL)
	{
		perror("Failed to allocate memory");
		exit(EXIT_FAILURE);
	}
	new_command->command_chain = cmd_chain;
	new_command->file = *file;
	new_command->next = NULL;
	if (*list_of_commands == NULL)
	{
		*list_of_commands = new_command;
	}
	else
	{
		current = *list_of_commands;
		while (current->next != NULL)
			current = current->next;
		current->next = new_command;
	}

	return (*list_of_commands);
}

t_command *parser_phase(t_lexer *lexer)
{
	t_command *list_of_commands;
	t_file *file;
	char **cmd_chain;
	t_command *current;
	int i;

	list_of_commands = NULL;
	file = NULL;
	cmd_chain = NULL;
	while (lexer)
	{
		if (lexer->type == WORD && lexer->prev == NULL)
			cmd_chain = add_cmd_chain_to_list(cmd_chain, lexer->value);
		else if (lexer->type == WORD && lexer->prev->type == REDIRECT_IN)
			file = add_file_to_list(&file, lexer->value, REDIRECT_IN);
		else if (lexer->type == WORD && lexer->prev->type == REDIRECT_OUT)
			file = add_file_to_list(&file, lexer->value, REDIRECT_OUT);
		else if (lexer->type == WORD && lexer->prev->type == REDIRECT_APPEND)
			file = add_file_to_list(&file, lexer->value, REDIRECT_APPEND);
		else if (lexer->type == WORD && lexer->prev->type == REDIRECT_INPUT)
			file = add_file_to_list(&file, lexer->value, REDIRECT_INPUT);
		else if (lexer->type == WORD)
			cmd_chain = add_cmd_chain_to_list(cmd_chain, lexer->value);
		else if (lexer->type == PIPE)
		{
			list_of_commands = add_command_to_list(&list_of_commands, cmd_chain,
												   &file);
			cmd_chain = NULL;
			file = NULL;
		}
		lexer = lexer->next;
	}
	list_of_commands = add_command_to_list(&list_of_commands, cmd_chain, &file);
	process_command_chain_and_files(list_of_commands);
	return (list_of_commands);
}
