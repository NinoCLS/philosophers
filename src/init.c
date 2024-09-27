/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 15:58:36 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/27 15:07:20 by nclassea         ###   ########.fr       */
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
	pthread_mutex_init(&data->write_mutex, NULL);
	pthread_mutex_init(&data->is_dead_mutex, NULL);
	pthread_mutex_init(&data->finish_eating, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	pthread_mutex_init(&data->general_mutex, NULL);
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
		data->philo[i].meals_eaten = 0;
		data->philo[i].last_meal = get_time_in_ms();
		data->philo[i].left_fork = &data->forks_mutex[i];
		data->philo[i].right_fork = &data->forks_mutex[(i + 1) % data->n_philo];
		i++;
	}
	return (1);
}

int	init_args(char **av, t_data *data)
{
	if (check_limits(av[1]) || check_limits(av[2])
		|| check_limits(av[3]) || check_limits(av[4]))
		return (0);
	if (av[5] && check_limits(av[5]))
		return (0);
	data->n_philo = ft_atoi(av[1]);
	data->t2d = ft_atoi(av[2]);
	data->t2e = ft_atoi(av[3]);
	data->t2s = ft_atoi(av[4]);
	if (av[5])
		data->nb_of_meals = ft_atoi(av[5]);
	else
		data->nb_of_meals = 0;
	if (check_limits2(data))
		return (0);
	data->is_dead = 0;
	data->philos_finished_eating = 0;
	return (1);
}
