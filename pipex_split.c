/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:59:58 by rpires-c          #+#    #+#             */
/*   Updated: 2024/10/03 17:11:41 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	count_words(const char *str, char c)
{
	int	i;
	int	n_words;
	int	inside_quotes;

	n_words = 0;
	inside_quotes = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
			inside_quotes = !inside_quotes;
		if (str[i] != c && !inside_quotes)
		{
			if (i == 0 || (str[i - 1] == c && !inside_quotes))
				n_words++;
		}
		i++;
	}
	return (n_words);
}

char	*extract_word(const char *str, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)ft_calloc((end - start + 1), sizeof(char));
	if (word == NULL)
		return (NULL);
	while (start < end)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

int	find_word_end(const char *s, int start, char c)
{
	int	inside_quotes;
	int	i;

	inside_quotes = 0;
	i = start;
	while (s[i] != '\0')
	{
		if (s[i] == '\'')
			inside_quotes = !inside_quotes;
		if (s[i] == c && !inside_quotes)
			return (i);
		i++;
	}
	return (i);
}

char	**allocate_result(int word_count)
{
	char	**res;

	res = (char **)ft_calloc((word_count + 1), sizeof(char *));
	if (!res)
		return (NULL);
	return (res);
}

char	**pipex_split(char const *s, char c)
{
	int		i;
	int		j;
	int		word_end;
	char	**res;

	i = -1;
	j = 0;
	if (!s)
		return (NULL);
	res = allocate_result(count_words(s, c));
	if (!res)
		return (NULL);
	while (s[++i] != '\0')
	{
		if (s[i] != c)
		{
			word_end = find_word_end(s, i, c);
			res[j++] = extract_word(s, i, word_end);
			i = word_end - 1;
		}
	}
	res[j] = NULL;
	return (res);
}

/* int main(void)
{
    char **res;
    int i = 0;

    res = pipex_split("lorem ipsum 'dolor sit' amet", ' ');
    while (res[i])
	{
        printf("%s\n", res[i]);
        free(res[i]);
        i++;
    }
    free(res);
    return 0;
} */
