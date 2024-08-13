/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:10:35 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/13 16:24:41 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>

#define INT_MAX 2147483647

typedef	struct s_philo t_philo;

typedef struct s_data
{
	int	n_philo;
	int	t2e;
	int	t2d;
	int	t2s;
	int	time_philo_must_e;
	int	is_dead;
	pthread_mutex_t *forks;
	t_philo *philo;
} t_data;

typedef	struct s_philo
{
	int id;
	int state;
	pthread_t thread;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	t_data *data;
} t_philo;




// UTILS
int	ft_atoi(const char *nptr);

#endif
