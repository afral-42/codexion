/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:42:42 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 23:07:20 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "codexion.h"

void	*exit_threads_init(pthread_t *threads, pthread_mutex_t *status, pthread_mutex_t *running)
{
	free(threads);
	if (status)
		pthread_mutex_destroy(status);
	if (running)
		pthread_mutex_destroy(running);
	return (NULL);
}

int	fill_args(t_sim *sim, t_coders_args *coders_args)
{
	size_t	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		coders_args[i].id = i;
		coders_args[i].sim = sim;
		coders_args[i].end = FALSE;
		if (pthread_mutex_init(&(coders_args[i].end_mutex), NULL))
		{
			clean_coders_mutexes(coders_args, i, i);
			return (1);
		}
		if (pthread_mutex_init(&(coders_args[i].last_compil_mutex), NULL))
		{
			clean_coders_mutexes(coders_args, i + 1, i);
			return (1);
		}
		i++;
	}
	return (0);
}

t_coders_args	*generate_args(t_sim *sim)
{
	t_coders_args	*coders_args;

	coders_args = malloc(sizeof(t_coders_args) * sim->config.number_of_coders);
	if (!coders_args)
		return (NULL);
	if (fill_args(sim, coders_args))
	{
		free(coders_args);
		return (NULL);
	}
	return (coders_args);
}

pthread_t	*init_threads(t_sim *sim, t_coders_args *args)
{
	size_t		i;
	pthread_t	*threads;

	threads = malloc(sizeof(pthread_t) * (sim->config.number_of_coders + 1));
	if (!threads)
		return (exit_threads_init(NULL, NULL, NULL));
	i = 0;
	sim->start_time = get_actual_time();
	sim->running = TRUE;
	if (pthread_mutex_init(&(sim->status_mutex), NULL))
		return (exit_threads_init(threads, NULL, NULL));
	if (pthread_mutex_init(&(sim->running_mutex), NULL))
		return (exit_threads_init(threads, &(sim->status_mutex), NULL));
	while (i < sim->config.number_of_coders)
	{
		args[i].last_compilation = sim->start_time;
		if (pthread_create(&(threads[i]), NULL, coder_routine, &args[i]))
			return (exit_threads_init(threads,
				&(sim->status_mutex), &(sim->running_mutex)));
		i++;
	}
	if (pthread_create(&(threads[i]), NULL, monitor_routine, args))
		return (exit_threads_init(threads,
			&(sim->status_mutex), &(sim->running_mutex)));
	return (threads);
}

void	wait_threads(pthread_t *threads, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}
