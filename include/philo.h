
#ifndef PHILO_H
# define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_config
{
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			num_of_philos;
	int			times_to_eat;
	long long	start_time;
}			t_config;

typedef struct s_program
{
	int				is_dead;
	pthread_t		monitor_thread;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*forks;
	t_config		*config;

}		t_program;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	long long		last_meal;
	int				meals_eaten;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_program		*program;
}			t_philo;

int			ft_atoi(const char *str);
int			parse_args(t_config *config, int argc, char **argv);
long long	get_time(void);
int			init_program(t_program *program, t_config *config);
void		destroy_program(t_program *program);
void		print_status(t_philo *philo, char *status);
void		ft_usleep(t_philo *philo, int ms);
void		take_forks(t_philo *philo);
void		drop_forks(t_philo *philo);
int			is_dead(t_philo *philo);
void		*monitor(void *arg);

#endif
