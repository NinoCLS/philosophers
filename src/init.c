/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nino <nino@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 15:58:36 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/30 14:07:45 by nino             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	init_mutex(t_data *data)
{
	int	i;

	i = -1;
	data->forks_mutex = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	if (!data->forks_mutex)
		return (0);
	while (++i < data->n_philo)
		pthread_mutex_init(&data->forks_mutex[i], NULL);
	pthread_mutex_init(&data->msg_mutex, NULL);
	return (1);
}

int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	data->philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philo)
		return (0);
	while (i < data->n_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].data = data;
		data->philo[i].left_fork = &data->forks_mutex[i];
		// right fork == next philo 
		data->philo[i].right_fork = &data->forks_mutex[(i + 1) % data->n_philo];
		i++;
	}
	return (1);
}

static int	check_args(t_data *data)
{
	if (data->nb_of_meals && (data->nb_of_meals < 60 || data->nb_of_meals > INT_MAX))
		return (1);
	if (data->n_philo < 1 || data->n_philo > 200 ||
		data->t2e < 60 || data->t2e > INT_MAX ||
		data->t2d < 60 || data->t2d > INT_MAX ||
		data->t2s < 60 || data->t2s > INT_MAX)
			return (1);
	return (0);
}

int	init_args(char **av, t_data *data)
{
	data->n_philo = ft_atoi(av[1]);
	data->t2e = ft_atoi(av[2]);
	data->t2d = ft_atoi(av[3]);
	data->t2s = ft_atoi(av[4]);
	if (av[5])
		data->nb_of_meals = ft_atoi(av[5]);
	else
		data->nb_of_meals = 0;
	data->is_dead = 0;
	data->start_time = get_time_in_ms();
	return (check_args(data));
}
