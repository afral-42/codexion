/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:03 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 23:38:18 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>

static int	exit_init(t_coders_args *args, t_dongle *dongles,
	pthread_t *threads)
{
	free(args);
	free(threads);
	free(dongles);
	return (1);
}

static int	exit_program(
	t_sim *sim, t_coders_args *args, pthread_t *threads, t_bool total
)
{
	clean_coders_mutexes(
		args, sim->config.number_of_coders, sim->config.number_of_coders);
	clean_dongle_mutexes(sim->dongles, sim->config.number_of_coders);
	if (total)
		clean_simulation_mutexes(sim);
	clean_dongle_conds(sim->dongles, sim->config.number_of_coders);
	exit_init(args, sim->dongles, threads);
	if (total)
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_coders_args	*args;
	pthread_t		*threads;
	t_sim			sim;

	if (parse_params(ac, av, &(sim.config)))
		return (1);
	args = generate_args(&sim);
	if (!args)
		return (exit_init(NULL, NULL, NULL));
	sim.dongles = init_dongles(sim.config.number_of_coders);
	if (!sim.dongles)
	{
		clean_coders_mutexes(args, sim.config.number_of_coders,
			sim.config.number_of_coders);
		return (exit_init(args, NULL, NULL));
	}
	threads = init_threads(&sim, args);
	if (!threads)
		return (exit_program(&sim, args, threads, FALSE));
	wait_threads(threads, sim.config.number_of_coders + 1);
	return (exit_program(&sim, args, threads, TRUE));
}
