/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmarkov <rmarkov@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 17:51:08 by rmarkov           #+#    #+#             */
/*   Updated: 2026/06/19 17:51:10 by rmarkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	parse_args(t_config *config, int argc, char **argv)
{
	config->num_of_philos = ft_atoi(argv[1]);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		config->times_to_eat = ft_atoi(argv[5]);
	else
		config->times_to_eat = -1;
	if (config->num_of_philos < 1 || config->time_to_die < 1
		|| config->time_to_eat < 1 || config->time_to_sleep < 1)
		return (-1);
		// TODO: check reqs
	if (argc == 6 && config->times_to_eat < 1)
		return (-1);
	return (0);
}
