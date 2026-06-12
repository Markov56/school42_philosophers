#include "include/philo.h"

int	check_death(t_philo *philo)
{
	long long	time_since_meal;
	long long	timestamp;

	pthread_mutex_lock(&philo->program->meal_lock);
	time_since_meal = get_time() - philo->last_meal;
	pthread_mutex_unlock(&philo->program->meal_lock);
	if (time_since_meal >= philo->program->config->time_to_die)
	{
		pthread_mutex_lock(&philo->program->write_lock);
		timestamp = get_time() - philo->program->config->start_time;
		printf("%lld %d died\n", timestamp, philo->id);
		pthread_mutex_unlock(&philo->program->write_lock);
		pthread_mutex_lock(&philo->program->dead_lock);
		philo->program->is_dead = 1;
		pthread_mutex_unlock(&philo->program->dead_lock);
		return (1);
	}
	return (0);
}

int	check_all_ate(t_philo *philos)
{
	int	i;

	if (philos[0].program->config->times_to_eat == -1)
		return (0);
	i = 0;
	while (i < philos[0].program->config->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].program->meal_lock);
		if (philos[i].meals_eaten < philos[0].program->config->times_to_eat)
		{
			pthread_mutex_unlock(&philos[i].program->meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&philos[i].program->meal_lock);
		i++;
	}
	return (1);
}

void	*monitor(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (!is_dead(philos))
	{
		i = 0;
		while (i < philos[0].program->config->num_of_philos)
		{
			if (check_death(&philos[i]))
				return (NULL);
			i++;
		}
		if (check_all_ate(philos))
		{
			pthread_mutex_lock(&philos[0].program->dead_lock);
			philos[0].program->is_dead = 1;
			pthread_mutex_unlock(&philos[0].program->dead_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}