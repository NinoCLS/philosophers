/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nino <nino@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:34:10 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/20 18:54:20 by nino             ###   ########.fr       */
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

int	lock_forks(t_philo *philo, pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	// pthread_mutex_lock(fork1);
	// if (!is_philo_dead(philo))
	// 	write_message(philo, "has taken a fork");
	// if (pthread_mutex_lock(fork2) != 0)
	// {
	// 	pthread_mutex_unlock(fork1);
	// 	return (1);
	// }
	// if (!is_philo_dead(philo))
	// 	write_message(philo, "has taken a fork");

	pthread_mutex_lock(fork1);
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
	pthread_mutex_lock(fork2);
	if (!is_philo_dead(philo))
		write_message(philo, "has taken a fork");
	
	return (0);
}



int take_forks(t_philo *philo)
{
	// int	res_lock;

	// if (philo->id % 2 == 0)
	// 	res_lock = lock_forks(philo, philo->right_fork, philo->left_fork);
	// else
	// 	res_lock = lock_forks(philo, philo->left_fork, philo->right_fork);
	// if (res_lock)
	// 	return (1);

if (philo->id % 2 == 0) {
    // Philosophes pairs : prennent la fourchette droite en premier
    pthread_mutex_lock(philo->right_fork);
    if (!is_philo_dead(philo))
        write_message(philo, "has taken a fork");

    usleep(100);  // Petit délai pour éviter les conflits

    pthread_mutex_lock(philo->left_fork);
    if (!is_philo_dead(philo))
        write_message(philo, "has taken a fork");
} 
else 
{
    // Philosophes impairs : prennent la fourchette gauche en premier
    pthread_mutex_lock(philo->left_fork);
    if (!is_philo_dead(philo))
        write_message(philo, "has taken a fork");

    usleep(100);  // Petit délai pour éviter les conflits

    pthread_mutex_lock(philo->right_fork);
    if (!is_philo_dead(philo))
        write_message(philo, "has taken a fork");
}

	// check si philo est mort et unlock les forks si c'est le cas
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
	usleep(philo->data->t2e * 1000);
	// printf("t2e: %d\n", philo->data->t2e);
	// peut etre lock et unlock pour mealseaten;
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

void	*routine(void *philos)
{
	t_philo *philo;

	philo = (t_philo *)philos;
	if (philo->id % 2 == 1)
		usleep(100);
	while (philo->data->is_dead == 0)
	{
		if (take_forks(philo))
			break ;
		if (philo_dining(philo))
			break ;
		if (philo->data->nb_of_meals > 0 && philo->meals_eaten == philo->data->nb_of_meals)
			break ;
		write_message(philo, "is sleeping");
		usleep(philo->data->t2s * 1000);
		write_message(philo, "is thinking");
	}
	return (0);
}

int	check_philo_death(t_data *data)
{
	int	i;
	int	time_since_last_meal;
	
	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_lock(&data->is_dead_mutex);
		time_since_last_meal = get_time_in_ms() - data->philo[i].last_meal;
		// printf("time since last meal: %d\n", time_since_last_meal);
		// printf("t2d: %d\n", data->t2d);
		if (time_since_last_meal >= data->t2d)
		{
			data->is_dead = 1;
			pthread_mutex_unlock(&data->is_dead_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%d %d died\n", get_time_in_ms() - data->start_time, data->philo[i].id);
			pthread_mutex_unlock(&data->write_mutex);
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
		if (check_philo_death(data) || check_philo_meals(data))
			break ;
		usleep(1000);
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

void	destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philo)
		pthread_mutex_destroy(&data->forks_mutex[i]);
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->is_dead_mutex);
	pthread_mutex_destroy(&data->finish_eating);
	free(data->forks_mutex);
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
	destroy_mutex(data);
	free_data(data, 2);
}
