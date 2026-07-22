/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 20:08:32 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 20:40:18 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_dongle_mutexes(t_sim *sim, t_coders_args *args)
{
	size_t	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&(sim->dongles[i].dongle_mutex));
		i++;
	}
}
void	clean_coders_mutexes(t_sim *sim, t_coders_args *args)
{
	size_t	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_destroy(&(args[i].end_mutex));
		i++;
	}
}

void	clean_simulation_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&(sim->running_mutex));
	pthread_mutex_destroy(&(sim->status_mutex));
}

void	clean_dongle_conds(t_sim *sim, t_coders_args *args)
{
	size_t	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_cond_destroy(&(sim->dongles[i].cond));
		i++;
	}
}