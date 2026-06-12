#include "include/philo.h"

t_philo *init_philos(t_program *program)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * program->config->num_of_philos);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < program->config->num_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].last_meal = program->config->start_time;
		philos[i].program = program;
		philos[i].l_fork = &program->forks[i];
		philos[i].r_fork = &program->forks[(i + 1) % program->config->num_of_philos];
		i++;
	}
	return (philos);
}

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	// while (!is_dead(philo))
	while (1) //
	{
		take_forks(philo);
		print_status(philo, "is eating");
		pthread_mutex_lock(&philo->program->meal_lock);
		philo->last_meal = get_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->program->meal_lock);
		ft_usleep(philo->program->config->time_to_eat);
		drop_forks(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->program->config->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

int	start_threads(t_philo *philos, t_program *program)
{
	int	i;

	i = 0;
	while (i < program->config->num_of_philos)
	{
		// philos[i].last_meal = program->config->start_time;
		if (pthread_create(&philos[i].thread, NULL, &philosopher, &philos[i]) != 0)
			return (-1);
		i++;
	}
	if (pthread_create(&program->monitor_thread, NULL, &monitor, philos) != 0)
		return (-1);
	return (0);
}

void	join_threads(t_philo *philos, t_program *program)
{
	int	i;

	i = 0;
	while (i < program->config->num_of_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(program->monitor_thread, NULL);
}

// void	*monitor(void *arg)
// {
// 	t_philo		*philos;
// 	int			i;
// 	long long	time_since_meal;
// 	int			all_ate;
// 	long long	timestamp;

// 	philos = (t_philo *)arg;
// 	while (!is_dead(philos))
// 	{
// 		i = 0;
// 		all_ate = 1;
// 		while (i < philos[0].program->config->num_of_philos)
// 		{
// 			pthread_mutex_lock(&philos[i].program->meal_lock);
// 			time_since_meal = get_time() - philos[i].last_meal;
// 			pthread_mutex_unlock(&philos[i].program->meal_lock);
// 			if (time_since_meal > philos[i].program->config->time_to_die)
// 			{
// 				pthread_mutex_lock(&philos[i].program->write_lock);
// 				timestamp = get_time() - philos[i].program->config->start_time;
// 				printf("%lld %d died\n", timestamp, philos[i].id);
// 				pthread_mutex_unlock(&philos[i].program->write_lock);
// 				pthread_mutex_lock(&philos[i].program->dead_lock);
// 				philos[i].program->is_dead = 1;
// 				pthread_mutex_unlock(&philos[i].program->dead_lock);
// 				return (NULL);
// 			}
// 			if (philos[0].program->config->times_to_eat != -1)
// 			{
// 				pthread_mutex_lock(&philos[i].program->meal_lock);
// 				if (philos[i].meals_eaten < philos[0].program->config->times_to_eat)
// 					all_ate = 0;
// 				pthread_mutex_unlock(&philos[i].program->meal_lock);
// 			}
// 			i++;
// 		}
// 		if (all_ate && philos[0].program->config->times_to_eat != -1)
// 		{
// 			pthread_mutex_lock(&philos[0].program->dead_lock);
// 			philos[0].program->is_dead = 1;
// 			pthread_mutex_unlock(&philos[0].program->dead_lock);
// 			return (NULL);
// 		}
// 		usleep(1000);
// 	}
// 	return (NULL);
// }

int	main(int argc, char **argv)
{
	t_config	config;
	t_program	program;
	t_philo		*philos;

	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments");
		return (1);
	}
	if (parse_args(&config, argc, argv) == -1)
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	if (init_program(&program, &config) == -1)
	{
		printf("Error: initialization failed\n");
		return (1);
	}
	// config.start_time = get_time();
	philos = init_philos(&program);
	if (!philos)
	{
		printf("Error: philos initialization failed\n");
		// TODO: clean
		return (1);
	}
	if (start_threads(philos, &program) == -1)
	{
		printf("Error: thread creation failed\n");
		return (1);
	}
	join_threads(philos, &program);
	destroy_program(&program);
	free(philos);
	return (0);
}