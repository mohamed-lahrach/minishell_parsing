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
				return (1);
			}
		}
		else if (current->type == REDIRECT_OUT)
		{
			if (current->next == NULL || current->next->type != WORD)
			{
				printf("bash: syntax error near unexpected token `newline' redirect out at the end or (redirect out followed sepecial char.)\n");
				return (1);
			}
		}
		else if (current->type == REDIRECT_IN)
		{
			if (current->next == NULL || current->next->type != WORD)
			{
				printf("bash: syntax error near unexpected token `newline' redirect in at the end or (redirect in followed sepecial char.)\n");
				return (1);
			}
		}
		else if (current->type == REDIRECT_APPEND)
		{
			if (current->next == NULL || current->next->type != WORD)
			{
				printf("bash: syntax error near unexpected token `newline' redirect append at the end or (redirect append followed sepecial char.)\n");
				return (1);
			}
		}
		else if (current->type == REDIRECT_INPUT)
		{
			if (current->next == NULL || current->next->type != WORD)
			{
				printf("bash: syntax error near unexpected token `newline' redirect input at the end or (redirect input followed sepecial char.)\n");
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}