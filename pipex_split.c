/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:59:58 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/17 17:01:42 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "pipex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	count_quotes(const char *str)
{
	int	quote_counter;
	int i;

	quote_counter = 0;
	i = -1;
	while (str[++i] != 0)
	{
		if (str[i] == '\'')
			quote_counter++;
	}
	return (quote_counter / 2);
}

static int	word_count(const char *str, char c)
{
	int	i;
	int	flag;
	int	n_words;
	int inside_quotes;

	i = 0;
	flag = 0;
	n_words = 0;
	inside_quotes = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			inside_quotes = !inside_quotes;

		if (str[i] != c && flag == 0 && !inside_quotes)
		{
			flag = 1;
			n_words++;
		}
		else if (str[i] == c && !inside_quotes)
			flag = 0;
		i++;
	}
	return (n_words);
}

static char	*str_length(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)calloc((end - start + 1), sizeof(char));
	while (start < end)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		index;
	int		inside_quotes;
	char	**res;

	i = 0;
	j = 0;
	index = -1;
	inside_quotes = 0;
	res = (char **)calloc((word_count(s, c) + 1), sizeof(char *));
	if (!s || !res)
		return (NULL);
	while (i <= strlen(s))
	{
		if (s[i] == '\'')
			inside_quotes = 1;
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == strlen(s)) && index >= 0 && inside_quotes == 1)
		{
			res[j++] = str_length(s, index, i);
			index = -1;
		}
		i++;
	}
	res[j] = 0;
	return (res);
}

int	main(void)
{
	char	**res;
	int		i;

	i = 0;
	res = ft_split("lorem ipsum 'dolor sit' amet", ' ');
	while (res[i])
	{
		printf("%s\n", res[i]);
		i++;
	}
	return (0);
}
