/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:59:58 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/19 20:30:07 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int pipex_split_word_count(const char *str, char c)
{
    int i;
    int n_words;
    int inside_quotes;

	i = -1;
	n_words = 0;
	inside_quotes = 0;
    while (str[++i] != '\0')
    {
        if (str[i] == '\'')
            inside_quotes = !inside_quotes;
        if (str[i] != c && !inside_quotes)
        {
            if (i == 0 || (str[i - 1] == c && !inside_quotes))
                n_words++;
        }
    }
    return n_words;
}

static char *pipex_split_str_length(const char *str, int start, int end)
{
    char *word;
    int i;

	i = 0;
    word = (char *)ft_calloc((end - start + 1), sizeof(char));
    if (word == NULL)
        return NULL;
    while (start < end)
    {
        word[i] = str[start];
        i++;
        start++;
    }
    word[i] = '\0';
    return word;
}

char **pipex_split(char const *s, char c)
{
    size_t i;
    size_t j;
    int index;
    int inside_quotes;
    char **res;

	i = 0;
	j = 0;
	index = -1;
	inside_quotes = 0;
    res = (char **)ft_calloc((pipex_split_word_count(s, c) + 1), sizeof(char *));
    if (!s || !res)
        return (NULL);
    while (i <= ft_strlen(s))
	{
        if (s[i] == '\'')
            inside_quotes = !inside_quotes;
        if (s[i] != c && index < 0)
            index = i;
        else if ((s[i] == c || s[i] == '\0') && index >= 0)
		{
            if (!inside_quotes)
			{
                res[j++] = pipex_split_str_length(s, index, i);
                index = -1;
            }
        }
        i++;
    }
    if (index >= 0)
        res[j++] = pipex_split_str_length(s, index, i);
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
