#include "minishell.h"

void remove_quotes(char **str)
{
	int i = 0;
	int j = 0;
	char current_quote;
	char *new_str = (char *)malloc(strlen(*str) + 1);
	if (new_str == NULL)
	{
		free(new_str);
		return;
	}

	while ((*str)[i])
	{
		if ((*str)[i] == '\'' || (*str)[i] == '"')
		{
			current_quote = (*str)[i];
			i++;
			while ((*str)[i] && (*str)[i] != current_quote)
				new_str[j++] = (*str)[i++];
			if ((*str)[i] == current_quote)
				i++;
		}
		else
			new_str[j++] = (*str)[i++];
	}
	new_str[j] = '\0';
	free(*str);
	*str = new_str;
}

void process_command_chain_and_files(t_command *command)
{
	int i;
	t_file *file;

	while (command)
	{
		i = 0;
		if (command->command_chain != NULL)
		{
			while (command->command_chain[i] != NULL)
			{
				remove_quotes(&command->command_chain[i]);
				i++;
			}
		}
		file = command->file;
		while (file)
		{
			remove_quotes(&file->file_name);
			file = file->next;
		}
		command = command->next;
	}
}
