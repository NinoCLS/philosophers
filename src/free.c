/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:54:49 by nclassea          #+#    #+#             */
/*   Updated: 2024/10/08 13:52:22 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&data->forks_mutex[i]);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->is_dead_mutex);
	pthread_mutex_destroy(&data->finish_eating);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->general_mutex);
	free(data->forks_mutex);
}

int	free_data(t_data *data, int msg)
{
	if (msg == 0)
	{
		printf("Malloc Error\n");
		free(data);
		return (0);
	}
	if (msg == 1)
	{
		printf("Invalid arguments\n");
		free(data);
		return (0);
	}
	if (msg == 2)
	{
		free(data->philo);
		free(data);
		return (0);
	}
	return (0);
}
