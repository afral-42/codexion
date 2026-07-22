/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:05 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 23:40:08 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "codexion.h"

int	exit_parsing(char *err)
{
	printf("Error: %s\n", err);
	print_usage();
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

int	parse_params(int ac, char **av, t_config *config)
{
	size_t	bad;

	if (ac != 9)
		return (exit_parsing("invalid number of arguments (expected 8)"));
	bad = validate_numeric_args(av, 1, 7);
	if (bad)
	{
		printf("Error: argument %zu must be a positive integer: '%s'\n",
			bad, av[bad]);
		print_usage();
		return (1);
	}
	parse_config_numbers(config, av);
	if (parse_config_scheduler(config, av))
	{
		printf("Error: scheduler must be 'fifo' or 'edf' (lowercase)\n");
		print_usage();
		return (1);
	}
	return (0);
}
