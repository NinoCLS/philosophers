/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:59:49 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/24 17:26:38 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	write_message(t_philo *philo, char *msg)
{
	int	current_time;

	if (is_philo_dead(philo))
		return ;
	pthread_mutex_lock(&philo->data->write_mutex);
	current_time = get_time_in_ms() - philo->data->start_time;
	printf("%d %d %s\n", current_time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->write_mutex);
}

void	release_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	lock_forks(t_philo *philo, pthread_mutex_t *fork1,
	pthread_mutex_t *fork2)
{
	pthread_mutex_lock(fork1);
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
	pthread_mutex_lock(fork2);
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
}

int	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
		lock_forks(philo, philo->right_fork, philo->left_fork);
	else
		lock_forks(philo, philo->left_fork, philo->right_fork);
	if (is_philo_dead(philo))
	{
		release_forks(philo);
		return (1);
	}
	return (0);
}

int	philo_dining(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	write_message(philo, "is eating");
	ft_sleep(philo, philo->data->t2e);
	if (philo->data->nb_of_meals > 0)
	{
		philo->meals_eaten++;
		if (philo->meals_eaten == philo->data->nb_of_meals)
		{
			pthread_mutex_lock(&philo->data->finish_eating);
			philo->data->philos_finished_eating++;
			pthread_mutex_unlock(&philo->data->finish_eating);
		}
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (is_philo_dead(philo));
}
