/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:34:10 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/27 17:31:16 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	write_message(t_philo *philo, char *msg)
{
	long long	time;

	time = get_time_in_ms();
	printf("%d %d %s\n", time - philo->data->start_time, philo->id, msg);
}

int	is_philo_dead(t_philo *philo)
{
	long long	time;

	time = get_time_in_ms();
	if (time - philo->data->start_time > philo->data->t2d)
	{
		printf("%d %d died\n", time - philo->data->start_time, philo->id);
		return (1);
	}
	return (0);
}

int	take_forks(t_philo *philo)
{
	// si philo pair prend fourchette droite
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
	return 1;
}

void	*philo_routine(void *args)
{
	t_philo *philo;

	philo = (t_philo *)args;
	// int	i = 0;
	while (philo->data->is_dead == 0)
	{
		// take fork 
		take_forks(philo);
		// takes forks
		// eat
		// think
	}
	return (0);
}


void	start_routine(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_create(&data->philo[i].thread, NULL, &philo_routine, &data->philo[i]);
	i = -1;
	while (++i < data->n_philo)
		pthread_join(data->philo[i].thread, NULL);
}


int main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if ((ac != 5 && ac != 6) || (init_args(av, data)))
		free_data(data, 1);
	if (!init(data))
		free_data(data, 0);
	start_routine(data);
	// monitoring
	// join
	// free_philo
}