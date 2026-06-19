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
	if (philo->program->config->num_of_philos == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
		while (!is_dead(philo))
			usleep(500);
		pthread_mutex_unlock(philo->l_fork);
		return (NULL);
	}
	while (!is_dead(philo))
	{
		take_forks(philo);
		if (is_dead(philo))
		{
			drop_forks(philo);
			break ;
		}
		print_status(philo, "is eating");
		pthread_mutex_lock(&philo->program->meal_lock);
		philo->last_meal = get_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->program->meal_lock);
		ft_usleep(philo, philo->program->config->time_to_eat);
		drop_forks(philo);
		if (is_dead(philo))
			break ;
		print_status(philo, "is sleeping");
		ft_usleep(philo, philo->program->config->time_to_sleep);
		if (is_dead(philo))
			break ;
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
