#include "include/philo.h"

int	init_program(t_program *program, t_config *config)
{
	int	i;

	program->is_dead = 0;
	program->config = config;
	program->config->start_time = get_time();
	if (pthread_mutex_init(&program->dead_lock, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&program->write_lock, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&program->meal_lock, NULL) != 0)
		return (-1);
	program->forks = malloc(sizeof(pthread_mutex_t) * config->num_of_philos);
	if (!program->forks)
		return (-1);
	i = 0;
	while (i < config->num_of_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL) != 0)
			return (-1);
		i++;
	}
	return (0);
}