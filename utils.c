/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:10:01 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 19:56:18 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include "codexion.h"

size_t	get_actual_time(void)
{
	struct timeval	actual_time;

	gettimeofday(&actual_time, NULL);
	return ((actual_time.tv_sec * 1000) + (actual_time.tv_usec / 1000));
}

void	print_status(t_coders_args *coder_args, char *status, size_t waiting)
{
	size_t	sim_time;
	size_t	actual_time;

	pthread_mutex_lock(&(coder_args->sim->status_mutex));
	actual_time = get_actual_time();
	sim_time = actual_time - coder_args->sim->start_time;
	printf("%zu %d %s\n", sim_time, coder_args->id + 1, status);
	pthread_mutex_unlock(&(coder_args->sim->status_mutex));
	if (waiting)
		usleep(waiting * 1000);
}

int	sim_check(t_sim *sim)
{
	int	status;

	pthread_mutex_lock(&(sim->running_mutex));
	status = sim->running;
	pthread_mutex_unlock(&(sim->running_mutex));
	return (status);
}
