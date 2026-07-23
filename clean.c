/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 20:08:32 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/23 18:28:41 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "codexion.h"

void	clean_dongle_mutexes(
	t_dongle *dongles, size_t size
)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&(dongles[i].dongle_mutex));
		i++;
	}
}

void	clean_coders_mutexes(
	t_thread_args *args, size_t end_size, size_t compil_size
)
{
	size_t	i;

	i = 0;
	while (i < end_size)
	{
		pthread_mutex_destroy(&(args[i].end_mutex));
		i++;
	}
	while (i < compil_size)
	{
		pthread_mutex_destroy(&(args[i].last_compil_mutex));
		i++;
	}
}

void	clean_simulation_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&(sim->print_mutex));
	pthread_mutex_destroy(&(sim->status_mutex));
}

void	*exit_dongle_init(
	t_dongle *dongles, size_t dongle_len
)
{
	clean_dongle_mutexes(dongles, dongle_len);
	free(dongles);
	return (NULL);
}

void	clean_threads_init(t_sim *sim, t_thread_args *args, size_t len)
{
	pthread_mutex_lock(&(sim->status_mutex));
	sim->status = ERROR;
	pthread_mutex_unlock(&(sim->status_mutex));
	wait_threads(args, len);
}
