/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:28:53 by nclassea          #+#    #+#             */
/*   Updated: 2024/09/27 15:03:25 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define LEN_MAX_INT 10
# define SIZE_MAX (18446744073709551615UL)

typedef struct s_philo	t_philo;
typedef struct s_data
{
	int				n_philo;
	int				t2e;
	int				t2d;
	int				t2s;
	int				nb_of_meals;
	int				is_dead;
	int				philos_finished_eating;
	int				start_time;
	pthread_mutex_t	*forks_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	is_dead_mutex;
	pthread_mutex_t	finish_eating;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	general_mutex;
	pthread_t		monitoring_thread;
	t_philo			*philo;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
	int				meals_eaten;
}	t_philo;

// PHILO_ACTIONS
int			philo_dining(t_philo *philo);
int			take_forks(t_philo *philo);
void		write_message(t_philo *philo, char *msg);

// UTILS
int			ft_atoi(const char *nptr);
int			get_time_in_ms(void);
void		ft_sleep(t_philo *philo, int time);
int			ft_strlen(char *s);

// FREE
void		destroy_mutex(t_data *data);
int			free_data(t_data *data, int msg);

// INIT
int			init(t_data *data);
int			init_args(char **av, t_data *data);
int			init_philo(t_data *data);
int			init_mutex(t_data *data);

// MOINTORING
void		*monitoring(void *arg);

// ROUTINE
void		create_routine(t_data *data);
int			is_philo_dead(t_philo *philo);

// PARSING
int			check_limits(char *str);
int			check_limits2(t_data *data);

#endif
