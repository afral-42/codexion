/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:42:42 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/23 20:27:25 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "codexion.h"

int	exit_threads_init(t_sim *sim, t_bool print, t_bool status)
{
	if (print)
		pthread_mutex_destroy(&(sim->print_mutex));
	if (status)
		pthread_mutex_destroy(&(sim->status_mutex));
	return (1);
}

int	fill_args(t_sim *sim, t_thread_args *coders_args)
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

t_thread_args	*generate_args(t_sim *sim)
{
	t_thread_args	*coders_args;

	coders_args = malloc(sizeof(t_thread_args)
			* (sim->config.number_of_coders + 1));
	if (!coders_args)
		return (NULL);
	if (fill_args(sim, coders_args))
	{
		free(coders_args);
		return (NULL);
	}
	return (coders_args);
}

int	init_threads(t_sim *sim, t_thread_args *args)
{
	size_t		i;

	i = -1;
	if (pthread_mutex_init(&(sim->print_mutex), NULL))
		return (exit_threads_init(sim, FALSE, FALSE));
	if (pthread_mutex_init(&(sim->status_mutex), NULL))
		return (exit_threads_init(sim, TRUE, FALSE));
	sim->start_time = get_actual_time();
	while (++i < sim->config.number_of_coders)
	{
		args[i].last_compilation = sim->start_time;
		if (pthread_create(&(args[i].thread), NULL, coder_routine, &args[i]))
		{
			clean_threads_init(sim, args, i);
			return (exit_threads_init(sim, TRUE, TRUE));
		}
	}
    pthread_mutex_lock(&(sim->status_mutex));
	sim->status = RUNNING;
    pthread_mutex_unlock(&(sim->status_mutex));
	if (pthread_create(&(args[i].thread), NULL, monitor_routine, args))
	{
		clean_threads_init(sim, args, i);
		return (exit_threads_init(sim, TRUE, TRUE));
	}
	return (0);
}

void	wait_threads(t_thread_args *args, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		pthread_join(args[i].thread, NULL);
		i++;
	}
}
