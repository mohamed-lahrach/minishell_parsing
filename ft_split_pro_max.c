/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pro_max.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamed <mohamed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:26:50 by oel-moue          #+#    #+#             */
/*   Updated: 2024/08/24 19:46:33 by mohamed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_words(char *s)
{
	int count = 0;
	int i = 0;
	bool in_quotes = false;

	while (s && s[i] != '\0')
	{
		while ((s[i] == ' ' || s[i] == '\t') && !in_quotes)
			i++;
		if (s[i] == '\0')
			break;
		count++;
		while (s[i] != '\0' && (in_quotes || (s[i] != ' ' && s[i] != '\t')))
		{
			if (s[i] == '"')
				in_quotes = !in_quotes;
			i++;
		}
	}
	return count;
}

static char	**free_list(char **str, int n)
{
	while (n >= 0)
	{
		free(str[n]);
		n--;
	}
	free(str);
	return (NULL);
}


char **ft_split_pro_max(char *s)
{
	char **ptr;
	int i;
	int j;
	int n;
	bool in_quotes;

	ptr = (char **)malloc((count_words(s) + 1) * sizeof(char *));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	n = 0;
	in_quotes = false;
	while (s && s[i] != '\0')
	{
		while ((s[i] == ' ' || s[i] == '\t') && !in_quotes)
			i++;
		if (s[i] == '\0')
			break;
		j = 0;
		while (s[i] != '\0' && (in_quotes || (s[i] != ' ' && s[i] != '\t')))
		{
			if (s[i] == '"')
				in_quotes = !in_quotes;
			i++;
			j++;
		}
		ptr[n++] = ft_substr(s, i - j, j);
		if (ptr[n - 1] == NULL)
			return (free_list(ptr, n - 1));
	}
	ptr[n] = NULL;
	return (ptr);
}
