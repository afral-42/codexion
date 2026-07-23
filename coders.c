/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 20:07:39 by anselme           #+#    #+#             */
/*   Updated: 2026/07/23 18:26:59 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "codexion.h"

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
		release_single_dongle(sim, right, actual_time, TRUE);
		release_single_dongle(sim, left, actual_time, TRUE);
	}
	else
	{
		release_single_dongle(sim, left, actual_time, TRUE);
		release_single_dongle(sim, right, actual_time, TRUE);
	}
}

int	get_dongles(t_sim *sim, int coder_id, t_thread_args *coder_args)
{
	t_dongle	*left;
	t_dongle	*right;

	left = &(sim->dongles[coder_id]);
	right = &(sim->dongles[(coder_id + 1) % sim->config.number_of_coders]);
	subscribe_dongle(sim, coder_id, left, last_ct(coder_args));
	subscribe_dongle(sim, coder_id, right, last_ct(coder_args));
	if (coder_id % 2 == 0)
		try_lock_dongles(left, right, coder_args);
	else
		try_lock_dongles(right, left, coder_args);
	print_status(coder_args, "has taken a dongle", 0);
	print_status(coder_args, "has taken a dongle", 0);
	return (0);
}

int	coder_actions(t_sim *sim, t_thread_args *coder_args)
{
	size_t		actual_time;

	actual_time = get_actual_time();
	pthread_mutex_lock(&(coder_args->last_compil_mutex));
	coder_args->last_compilation = actual_time;
	pthread_mutex_unlock(&(coder_args->last_compil_mutex));
	if (print_status(coder_args, "is compiling",
			sim->config.time_to_compile))
		return (1);
	release_dongles(sim, coder_args->id);
	if (print_status(coder_args, "is debugging",
			sim->config.time_to_debug))
		return (1);
	if (print_status(coder_args, "is refactoring",
			sim->config.time_to_refactor))
		return (1);
	return (0);
}

void	*coder_routine(void *args)
{
	t_thread_args	*coder_args;
	t_config		*conf;
	t_sim			*sim;
	size_t			compilations;

	compilations = -1;
	coder_args = (t_thread_args *)args;
	conf = &(coder_args->sim->config);
	sim = coder_args->sim;
	while (get_sim_status(sim) == WAITING)
	{
		if (get_sim_status(sim) == ERROR)
			return (NULL);
	}
	while (++compilations < conf->number_of_compiles_required && sim_check(sim))
	{
		if (get_dongles(sim, coder_args->id, coder_args))
			return (NULL);
		if (coder_actions(sim, coder_args))
			return (NULL);
	}
	pthread_mutex_lock(&(coder_args->end_mutex));
	coder_args->end = TRUE;
	pthread_mutex_unlock(&(coder_args->end_mutex));
	return (NULL);
}
