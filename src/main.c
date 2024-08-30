/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nino <nino@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:34:10 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/30 14:55:00 by nino             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"



int	is_philo_dead(t_philo *philo)
{
	int	time;

	time = get_time_in_ms();
	// if (time - philo->data->start_time > philo->data->t2d)
	// {
	// 	printf("%lld %d died\n", time - philo->data->start_time, philo->id);
	// 	return (1);
	// }
	if (philo->data->is_dead == 1)
	{
		printf("%lld %d died\n", time - philo->data->start_time, philo->id);
		return (1);
	}
	return (0);
}

void	write_message(t_philo *philo, char *msg)
{
	int	time;

	time = get_time_in_ms() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->msg_mutex);
	if (!is_philo_dead(philo))
		printf("%d %d %s\n", time, philo->id, msg);
	pthread_mutex_unlock(&philo->data->msg_mutex);
}

int	lock_forks(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	pthread_mutex_lock(fork1);
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
	if (pthread_mutex_lock(fork2) != 0)
	{
		pthread_mutex_unlock(fork1);
		return (1);
	}
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
	return (0);
}

int take_forks(t_philo *philo)
{
	int	res_lock;

	// lock les forks en fonction de l'id du philo
	if (philo->id % 2 == 0)
		res_lock = lock_forks(philo, philo->right_fork, philo->left_fork);
	else
		res_lock = lock_forks(philo, philo->left_fork, philo->right_fork);
	if (res_lock)
		return 1;

	// check si philo est mort et unlock les forks si c'est le cas
	if (is_philo_dead(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return 1;
	}
	return 0;
}


void	eat(t_philo *philo)
{
	write_message(philo, "is eating");
	usleep(philo->data->t2e);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
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
		eat(philo);
		// sleep
		// think
	}
	return (0);
}


void	create_routine(t_data *data)
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
	create_routine(data);
	// monitoring
	// join
	// free_philo
}
