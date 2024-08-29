/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nino <nino@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:34:10 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/29 20:10:09 by nino             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	write_message(t_philo *philo, char *msg)
{
	int	time;

	time = get_time_in_ms();
	printf("%lld %d %s\n", time - philo->data->start_time, philo->id, msg);
}

int	is_philo_dead(t_philo *philo)
{
	int	time;

	time = get_time_in_ms();
	if (time - philo->data->start_time > philo->data->t2d)
	{
		printf("%lld %d died\n", time - philo->data->start_time, philo->id);
		return (1);
	}
	else if (philo->data->is_dead == 1)
	{
		printf("%lld %d died\n", time - philo->data->start_time, philo->id);
		return (1);
	}
	return (0);
}

int take_forks(t_philo *philo)
{
	int lock_result;

	if (philo->id % 2 == 0)
	{
		lock_result = pthread_mutex_lock(philo->right_fork);
		if (lock_result != 0) return 1;
		if (!is_philo_dead(philo)) write_message(philo, "has taken a fork");

		lock_result = pthread_mutex_lock(philo->left_fork);
		if (lock_result != 0)
		{
			pthread_mutex_unlock(philo->right_fork);
			return 1;
		}
	}
	else
	{
		lock_result = pthread_mutex_lock(philo->left_fork);
		if (lock_result != 0) return 1;
		if (!is_philo_dead(philo)) write_message(philo, "has taken a fork");

		lock_result = pthread_mutex_lock(philo->right_fork);
		if (lock_result != 0)
		{
			pthread_mutex_unlock(philo->left_fork);
			return 1;
		}
	}
	if (is_philo_dead(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return 1;
	}
	return 0;
}



void	*routine(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	while (philo->data->is_dead == 0)
	{
		// take fork 
		take_forks(philo);
		// eat
		// sleep
		// think
	}
	return (0);
}


void	start_routine(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_create(&data->philo[i].thread, NULL, &routine, &data->philo[i]);
	i = -1;
	while (++i < data->n_philo)
		pthread_join(data->philo[i].thread, NULL);
}


int main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if ((ac != 5 && ac != 6) || (init_args(av, data)))
		return (free_data(data, 1));
	if (!init_mutex(data) || !init_philo(data))
		return (free_data(data, 0));
	start_routine(data);
	// monitoring
	// join
	// free_philo
}
