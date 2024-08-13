/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:34:10 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/13 16:40:33 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	free_data(t_data *data, int msg)
{
	if (msg == 1)
	{
		printf("Invalid arguemnts\n");
		free(data);
		exit(0);
		// free forks
		// free philo
	}
	if (msg == 0)
	{
		free(data);
		exit(0);
	}
}

int	check_args(t_data *data)
{
	if (data->time_philo_must_e && (data->time_philo_must_e < 60 || data->time_philo_must_e > INT_MAX))
		return (1);
	if (data->n_philo < 1 || data->n_philo > 200 ||
		data->t2e < 60 || data->t2e > INT_MAX ||
		data->t2d < 60 || data->t2d > INT_MAX ||
		data->t2s < 60 || data->t2s > INT_MAX)
			return (1);
	return (0);
}

int	init_args(char **av, t_data *data)
{
	data->n_philo = ft_atoi(av[1]);
	data->t2e = ft_atoi(av[2]);
	data->t2d = ft_atoi(av[3]);
	data->t2s = ft_atoi(av[4]);
	if (av[5])
		data->time_philo_must_e = ft_atoi(av[5]);
	return (check_args(data));
}

void	init_philo(t_data *data)
{
	int	i;

	i = 0;
	data->philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!data->philo)
		free_data(data, 0);
	// init mutex
	while (i < data->n_philo)
	{
		data->philo[i].id = i;
		data->philo[i].state = 0;
		data->philo[i].data = data;
		i++;
	}
}

void	init(t_data *data)
{
	init_philo(data);
	// init threads
	// join threads
	// free data
}


int main(int ac, char **av)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if ((ac != 5 && ac != 6) || (init_args(av, data)))
		free_data(data, 1);
	init(data);
}