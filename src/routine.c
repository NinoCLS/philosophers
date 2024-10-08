/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:58:17 by nclassea          #+#    #+#             */
/*   Updated: 2024/10/08 14:07:17 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	is_philo_dead(t_philo *philo)
{
	int	is_dead;

	pthread_mutex_lock(&philo->data->is_dead_mutex);
	is_dead = philo->data->is_dead;
	pthread_mutex_unlock(&philo->data->is_dead_mutex);
	return (is_dead);
}

int	solo_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->general_mutex);
	if (philo->data->n_philo == 1)
	{
		write_message(philo, "has taken a fork");
		pthread_mutex_unlock(&philo->data->general_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->general_mutex);
	return (0);
}

void	*routine(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (solo_philo(philo))
		return (0);
	if (philo->id % 2 == 1)
		ft_sleep(philo, 100);
	while (!is_philo_dead(philo))
	{
		if (take_forks(philo))
			break ;
		if (philo_dining(philo))
			break ;
		write_message(philo, "is sleeping");
		ft_sleep(philo, philo->data->t2s);
		write_message(philo, "is thinking");
		if (philo->data->n_philo % 2 == 1)
			ft_sleep(philo, philo->data->t2e * 2 - philo->data->t2s);
	}
	return (0);
}

void	create_routine(t_data *data)
{
	int	i;

	i = -1;
	data->start_time = get_time_in_ms();
	while (++i < data->n_philo)
		pthread_create(&data->philo[i].thread, NULL, &routine, &data->philo[i]);
	pthread_create(&data->monitoring_thread, NULL, &monitoring, data);
	i = -1;
	while (++i < data->n_philo)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(data->monitoring_thread, NULL);
}
