/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:56:47 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/24 17:20:39 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	check_philo_death(t_data *data)
{
	int	i;
	int	time_since_last_meal;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_lock(&data->is_dead_mutex);
		time_since_last_meal = get_time_in_ms() - data->philo[i].last_meal;
		if (time_since_last_meal > data->t2d)
		{
			data->is_dead = 1;
			pthread_mutex_unlock(&data->is_dead_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%d %d died\n", get_time_in_ms()
				- data->start_time, data->philo[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->is_dead_mutex);
		i++;
	}
	return (0);
}

static int	check_philo_meals(t_data *data)
{
	pthread_mutex_lock(&data->finish_eating);
	if (data->philos_finished_eating
		&& data->philos_finished_eating == data->n_philo)
	{
		pthread_mutex_lock(&data->is_dead_mutex);
		data->is_dead = 1;
		pthread_mutex_unlock(&data->is_dead_mutex);
		pthread_mutex_unlock(&data->finish_eating);
		return (1);
	}
	pthread_mutex_unlock(&data->finish_eating);
	return (0);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(&data->is_dead_mutex);
		if (data->is_dead)
		{
			pthread_mutex_unlock(&data->is_dead_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->is_dead_mutex);
		if (check_philo_death(data) || check_philo_meals(data))
			break ;
	}
	return (0);
}
