/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 15:02:52 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/27 15:09:31 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	is_char_in(char *str, int *i)
{
	while (str[*i])
	{
		if (str[*i] < '0' || str[*i] > '9')
			return (1);
		(*i)++;
	}
	return (0);
}

int	check_limits(char *str)
{
	int	i;
	int	len;

	len = ft_strlen(str);
	i = 0;
	if (str[0] == '+')
	{
		str++;
		len--;
	}
	if (is_char_in(str, &i))
		return (1);
	if (len < LEN_MAX_INT)
		return (0);
	i = 0;
	if (len == LEN_MAX_INT)
	{
		while (str[i] - "2147483647"[i] <= 0 && str[i])
			i++;
	}
	if (str[i] || len > LEN_MAX_INT)
		return (1);
	return (0);
}

int	check_limits2(t_data *data)
{
	if (data->n_philo < 1 || data->n_philo > 200)
		return (1);
	if (data->t2d < 60 || data->t2e < 60 || data->t2s < 60)
		return (1);
	return (0);
}
