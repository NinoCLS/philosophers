/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nino <nino@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:10:35 by nclassea          #+#    #+#             */
/*   Updated: 2024/08/30 14:32:51 by nino             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

#define INT_MAX 2147483647
#define SIZE_MAX (18446744073709551615UL)

typedef	struct s_philo t_philo;

typedef struct s_data
{
	int				n_philo;
	int				t2e;
	int				t2d;
	int				t2s;
	int				nb_of_meals;
	int				is_dead;
	long long		start_time;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	msg_mutex;
	t_philo			*philo;
} t_data;

typedef	struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
} t_philo;


// UTILS
int			ft_atoi(const char *nptr);
int			free_data(t_data *data, int msg);
int			get_time_in_ms();


// INIT
int			init(t_data *data);
int			init_args(char **av, t_data *data);
int			init_philo(t_data *data);
int			init_mutex(t_data *data);

#endif
