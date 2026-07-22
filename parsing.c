/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:05 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 20:45:16 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "codexion.h"

int	exit_parsing(char *err)
{
	printf("Error: %s", err);
	return (1);
}

int	is_number(char *s)
{
	if (*s == '\0')
		return (0);
	while (*s != '\0')
	{
		if (!(*s >= '0' && *s <= '9'))
			return (0);
		s++;
	}
	return (1);
}

void	parse_config_numbers(t_config *config, char **av)
{
	config->number_of_coders = atoi(av[1]);
	config->time_to_burnout = atoi(av[2]);
	config->time_to_compile = atoi(av[3]);
	config->time_to_debug = atoi(av[4]);
	config->time_to_refactor = atoi(av[5]);
	config->number_of_compiles_required = atoi(av[6]);
	config->dongle_cooldown = atoi(av[7]);
}

int	parse_config_scheduler(t_config *config, char **av)
{
	if (!strcmp(av[8], "edf"))
		config->scheduler = EDF;
	else if (!strcmp(av[8], "fifo"))
		config->scheduler = FIFO;
	else
		return (1);
	return (0);
}

int	parse_params(int ac, char **av, t_config *config)
{
	size_t	i;

	i = 1;
	if (ac != 9)
		return (exit_parsing("invalid arguments"));
	while (i < 8)
	{
		if (!is_number(av[i]))
			return (exit_parsing("time arguments musts be positive numbers"));
		i++;
	}
	parse_config_numbers(config, av);
	if (parse_config_scheduler(config, av))
		return (exit_parsing("scheduler must be fifo or edf"));
	return (0);
}
