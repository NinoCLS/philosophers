/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:52:26 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/27 17:31:00 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	free_data(t_data *data, int msg)
{
	if (msg == 1)
	{
		printf("Invalid arguemnts\n");
		free(data);
		return (0);
	}
	if (msg == 0)
	{
		printf("Malloc Error\n");
		free(data);
		return (0);
	}
	return (0);
}

long long get_time_in_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}


int	ft_atoi(const char *nptr)
{
	int	sign;
	int	out;

	out = 0;
	sign = 0;
	while ((*nptr >= 9 && *nptr <= 13) || (*nptr == 32))
		nptr++;
	if (*nptr == '-')
	{
		sign++;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	while (*nptr >= 48 && *nptr <= 57)
	{
		out *= 10;
		out += *nptr - 48;
		nptr++;
	}
	if (sign == 1)
		out *= -1;
	return (out);
}
