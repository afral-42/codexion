/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 20:07:39 by anselme           #+#    #+#             */
/*   Updated: 2026/07/22 21:31:08 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "codexion.h"

static void release_single_dongle(
	t_sim *sim, size_t coder_id, t_dongle *dongle, size_t time
)
{
	pthread_mutex_lock(&(dongle->dongle_mutex));
	dongle->available_at = time + sim->config.dongle_cooldown;
	dongle->held = FALSE;
	pthread_cond_broadcast(&(dongle->cond));
	pthread_mutex_unlock(&(dongle->dongle_mutex));
}

static void	release_dongles(t_sim *sim, size_t coder_id)
{
	t_dongle	*left;
	t_dongle	*right;
	size_t		actual_time;

	actual_time = get_actual_time();
	left = &(sim->dongles[coder_id]);
	right = &(sim->dongles[(coder_id + 1) % sim->config.number_of_coders]);
	if (coder_id % 2 == 0)
	{
		release_single_dongle(sim, coder_id, left, actual_time);
		release_single_dongle(sim, coder_id, right, actual_time);
	}
	else
	{
		release_single_dongle(sim, coder_id, right, actual_time);
		release_single_dongle(sim, coder_id, left, actual_time);
	}
}

int	get_dongles(t_sim *sim, int coder_id, t_coders_args *coder_args)
{
	t_dongle	*left;
	t_dongle	*right;

	left = &(sim->dongles[coder_id]);
	right = &(sim->dongles[(coder_id + 1) % sim->config.number_of_coders]);
	if (coder_id % 2 == 0)
	{
		if (lock_dongle(sim, coder_id, left))
			return (1);
		print_status(coder_args, "has taken a dongle", 0);
		if (lock_dongle(sim, coder_id, right))
			return (1);
		print_status(coder_args, "has taken a dongle", 0);
	}
	else
	{
		if (lock_dongle(sim, coder_id, right))
			return (1);
		print_status(coder_args, "has taken a dongle", 0);
		if (lock_dongle(sim, coder_id, left))
			return (1);
		print_status(coder_args, "has taken a dongle", 0);
	}
	return (0);
}

void	coder_actions(t_sim *sim, t_coders_args *coder_args)
{
	print_status(coder_args, "is compiling", sim->config.time_to_compile);
	release_dongles(sim, coder_args->id);
	print_status(coder_args, "is debugging", sim->config.time_to_debug);
	print_status(coder_args, "is refactoring", sim->config.time_to_refactor);
}

void	*coder_routine(void *args)
{
	t_coders_args	*coder_args;
	t_config		*conf;
	t_sim			*sim;
	size_t			compilations;

	compilations = 0;
	coder_args = (t_coders_args *)args;
	conf = &(coder_args->sim->config);
	sim = coder_args->sim;
	while (compilations < conf->number_of_compiles_required && sim_check(sim))
	{
		if (get_dongles(sim, coder_args->id, coder_args))
			return (NULL);
		coder_actions(sim, coder_args);
		compilations++;
	}
	pthread_mutex_lock(&(coder_args->end_mutex));
	coder_args->end = TRUE;
	pthread_mutex_unlock(&(coder_args->end_mutex));
	return (NULL);
}
