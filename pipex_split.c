/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:59:58 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/17 16:25:37 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "pipex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int word_count(const char *str, char c)
{
    int i = 0;
    int flag = 0;
    int n_words = 0;

    while (str[i] != '\0')
    {
        if (str[i] != c && flag == 0)
        {
            flag = 1;
            n_words++;
        }
        else if (str[i] == c)
            flag = 0;
        i++;
    }
    return n_words;
}

static char *str_length(const char *str, int start, int end)
{
    char *word;
    int i = 0;

    word = (char *)calloc((end - start + 1), sizeof(char));
    while (start < end)
        word[i++] = str[start++];
    word[i] = '\0';
    return word;
}

char **ft_split(char const *s, char c)
{
    size_t i = 0;
    size_t j = 0;
    int index = -1;
    char **res;

    res = (char **)calloc((word_count(s, c) + 1), sizeof(char *));
    if (!s || !res)
        return NULL;

    while (i <= strlen(s))
    {
        if (s[i] != c && index < 0)
            index = i;
        else if ((s[i] == c || i == strlen(s)) && index >= 0)
        {
            res[j++] = str_length(s, index, i);
            index = -1;
        }
        i++;
    }
    res[j] = 0;
    return res;
}

int main()
{
    char **res;
    int i = 0;

    res = ft_split("lorem ipsum \"dolor sit\" amet", ' ');

    while (res[i])
    {
        printf("%s\n", res[i]);
        i++;
    }
    return 0;
}
