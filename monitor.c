/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 12:51:09 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 23:27:41 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "codexion.h"

size_t	last_ct(t_coders_args *args)
{
	size_t	last_compilation;

	pthread_mutex_lock(&(args->last_compil_mutex));
	last_compilation = args->last_compilation;
	pthread_mutex_unlock(&(args->last_compil_mutex));
	return (last_compilation);
}

static void	*signal_to_coders(t_sim *sim, t_bool error)
{
	size_t	i;

	pthread_mutex_lock(&(sim->running_mutex));
	sim->running = FALSE;
	pthread_mutex_unlock(&(sim->running_mutex));
	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&(sim->dongles[i].dongle_mutex));
		pthread_cond_broadcast(&(sim->dongles[i].cond));
		pthread_mutex_unlock(&(sim->dongles[i].dongle_mutex));
		i++;
	}
	return (((void *)error));
}

static int	check_all_coders(t_sim *sim, t_coders_args *coders, size_t time)
{
	size_t	i;
	size_t	finished;

	i = 0;
	finished = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&(coders[i].end_mutex));
		if (coders[i].end)
			finished++;
		else if (last_ct(&(coders[i])) + sim->config.time_to_burnout <= time)
		{
			print_status(&(coders[i]), "burned out", 0);
			pthread_mutex_unlock(&(coders[i].end_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(coders[i].end_mutex));
		i++;
	}
	if (finished == sim->config.number_of_coders)
		return (2);
	return (0);
}

void	*monitor_routine(void *args)
{
	t_coders_args	*coders;
	t_sim			*sim;
	int				status;

	coders = (t_coders_args *)args;
	sim = coders[0].sim;
	while (1)
	{
		status = check_all_coders(sim, coders, get_actual_time());
		if (status == 1)
			return (signal_to_coders(sim, TRUE));
		if (status == 2)
			return (signal_to_coders(sim, FALSE));
		usleep(1000);
	}
	return (NULL);
}
