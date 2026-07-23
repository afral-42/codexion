/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:10:01 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/23 18:30:07 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include "codexion.h"
#include <string.h>

size_t	get_actual_time(void)
{
	struct timeval	actual_time;

	gettimeofday(&actual_time, NULL);
	return ((actual_time.tv_sec * 1000) + (actual_time.tv_usec / 1000));
}

int	print_status(t_thread_args *coder_args, char *status, size_t waiting)
{
	size_t	sim_time;
	size_t	actual_time;

	pthread_mutex_lock(&(coder_args->sim->print_mutex));
	if (sim_check(coder_args->sim) || !strcmp(status, "burned out"))
	{
		actual_time = get_actual_time();
		sim_time = actual_time - coder_args->sim->start_time;
		printf("%zu %d %s\n", sim_time, coder_args->id + 1, status);
	}
	pthread_mutex_unlock(&(coder_args->sim->print_mutex));
	if (!sim_check(coder_args->sim))
		return (1);
	while (waiting)
	{
		if (!sim_check(coder_args->sim))
			return (1);
		usleep(1000);
		waiting--;
	}
	return (0);
}

t_bool	sim_check(t_sim *sim)
{
	int	status;

	pthread_mutex_lock(&(sim->status_mutex));
	status = sim->status;
	pthread_mutex_unlock(&(sim->status_mutex));
	return (status == RUNNING);
}

void	print_usage(void)
{
	printf("Usage: ./codexion number_of_coders time_to_burnout\n");
	printf("       time_to_compile time_to_debug time_to_refactor\n");
	printf("       number_of_compiles_required dongle_cooldown scheduler\n");
}

int	validate_numeric_args(char **av, size_t start, size_t end)
{
	size_t	i;

	i = start;
	while (i <= end)
	{
		if (!is_number(av[i]))
			return (i);
		i++;
	}
	return (0);
}
