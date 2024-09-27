/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:52:26 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/27 14:51:22 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	ft_sleep(t_philo *philo, int time)
{
	int	start_time;

	start_time = get_time_in_ms();
	while ((get_time_in_ms() - start_time) < time)
	{
		usleep(1000);
		if (is_philo_dead(philo))
			return ;
	}
}

int	get_time_in_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
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

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '+')
		s++;
	while (s[i])
		i++;
	return (i);
}
