/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:03 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 21:23:01 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>

void	print_config(t_config *config)
{
	if (!config)
		return ;
	printf("=== ÉTAT DE LA CONFIGURATION ===\n");
	printf("Nombre de codeurs   : %zu\n", config->number_of_coders);
	printf("Temps avant burnout : %zu ms\n", config->time_to_burnout);
	printf("Temps de compilo    : %zu ms\n", config->time_to_compile);
	printf("Temps de debug      : %zu ms\n", config->time_to_debug);
	printf("Temps de refacto    : %zu ms\n", config->time_to_refactor);
	printf(
		"Compilations requises : %zu\n", config->number_of_compiles_required
	);
	printf("Cooldown du dongle  : %zu ms\n", config->dongle_cooldown);
	printf("Scheduler           : %d\n", config->scheduler);
	printf("================================\n");
}

static int	exit_init(t_coders_args *args, t_dongle *dongles, pthread_t *threads)
{
	free(args);
	free(threads);
	free(dongles);
	return (1);
}

static int	exit_program(
	t_sim *sim, t_coders_args *args, pthread_t *threads
)
{
	clean_coders_mutexes(args, sim->config.number_of_coders);
	clean_dongle_mutexes(sim->dongles, sim->config.number_of_coders);
	clean_simulation_mutexes(sim);
	clean_dongle_conds(sim->dongles, sim->config.number_of_coders);
	exit_init(args, sim->dongles, threads);
	return (0);
}

int	main(int ac, char **av)
{
	t_coders_args	*args;
	pthread_t		*threads;
	t_sim			sim;

	if (parse_params(ac, av, &(sim.config)))
		return (1);
	//print_config(&(sim.config));
	args = generate_args(&sim);
	if (!args)
		return (exit_init(NULL, NULL, NULL));
	sim.dongles = init_dongles(sim.config.number_of_coders);
	if (!sim.dongles)
	{
		clean_coders_mutexes(args, sim.config.number_of_coders);
		return (exit_init(args, NULL, NULL));
	}
	threads = init_threads(&sim, args);
	if (!threads)
	{
		clean_coders_mutexes(args, sim.config.number_of_coders);
		clean_dongle_mutexes(sim.dongles, sim.config.number_of_coders);
		clean_dongle_conds(sim.dongles, sim.config.number_of_coders);
		return (exit_init(args, sim.dongles, NULL));
	}
	wait_threads(threads, sim.config.number_of_coders + 1);
	exit_program(&sim, args, threads);
}
