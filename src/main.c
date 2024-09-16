/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:34:10 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/16 17:57:20 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	is_philo_dead(t_philo *philo)
{
	int	time;

	time = get_time_in_ms();
	if (philo->data->is_dead == 1)
	{
		printf("%d %d died\n", time - philo->data->start_time, philo->id);
		return (1);
	}
	return (0);
}

void	write_message(t_philo *philo, char *msg)
{
	int	time;

	time = get_time_in_ms() - philo->data->start_time;
	if (!is_philo_dead(philo))
	{
		
		pthread_mutex_lock(&philo->data->write_mutex);
		printf("%d %d %s\n", time, philo->id, msg);
		pthread_mutex_unlock(&philo->data->write_mutex);
	}
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


void	philo_dining(t_philo *philo)
{
	write_message(philo, "is eating");
	philo->last_meal = get_time_in_ms() - philo->data->start_time;
	printf("last_meal: %d\n", philo->last_meal);
	usleep(philo->data->t2e);
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
}

void	*routine(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	if (philo->id % 2 == 0)
		usleep(100);
	while (philo->data->is_dead == 0)
	{
		take_forks(philo);
		philo_dining(philo);
		if (philo->data->nb_of_meals > 0 && philo->meals_eaten == philo->data->nb_of_meals)
			break ;
		write_message(philo, "is sleeping");
		usleep(philo->data->t2s);
		write_message(philo, "is thinking");
		// printf("finished eating: %d\n", philo->data->philos_finished_eating);
	}
	return (0);
}

int	check_philo_death(t_data *data)
{
	int	i;
	int	time;

	time = get_time_in_ms();
	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_lock(&data->is_dead_mutex);
		if ((time - data->philo[i].last_meal) > data->t2d)
		{
			write_message(&data->philo[i], "died");
			data->is_dead = 1;
			pthread_mutex_unlock(&data->is_dead_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->is_dead_mutex);
		i++;
	}
	return (0);
}


int	check_philo_meals(t_data *data)
{
	pthread_mutex_lock(&data->is_dead_mutex);
	if (data->philos_finished_eating && data->philos_finished_eating == data->n_philo)
	{
		printf("All philos have eaten enough\n");
		data->is_dead = 1;
		pthread_mutex_unlock(&data->is_dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->is_dead_mutex);
	return (0);
}

void	*monitoring(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_philo_meals(data))
			break ;
		if (check_philo_death(data))
			break ;
		usleep(300);
	}
	return (0);
}


void	create_routine(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
	{
		pthread_create(&data->philo[i].thread, NULL, &routine, &data->philo[i]);
	}
	pthread_create(&data->monitoring_thread, NULL, &monitoring, data);
	i = -1;
	while (++i < data->n_philo)
		pthread_join(data->philo[i].thread, NULL);
	pthread_join(data->monitoring_thread, NULL);
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
	// join
	// free_philo
}
