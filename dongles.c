/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:44:18 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 23:23:38 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "codexion.h"
#include <stdio.h>

static int	check_disponibility(int coder_id, t_dongle *dongle)
{
	size_t	actual_time;

	actual_time = get_actual_time();
	return (dongle->queue[0].coder_id == coder_id
		&& dongle->available_at <= actual_time
		&& dongle->held == FALSE
	);
}

static int	wait_dongle(t_sim *sim, int coder_id, t_dongle *dongle)
{
	size_t	actual_time;
	size_t	duration;

	while (!check_disponibility(coder_id, dongle) && sim_check(sim))
	{
		if (dongle->held == TRUE)
			pthread_cond_wait(&(dongle->cond), &(dongle->dongle_mutex));
		actual_time = get_actual_time();
		if (dongle->available_at > actual_time)
		{
			duration = (dongle->available_at - actual_time) * 1000;
			pthread_mutex_unlock(&(dongle->dongle_mutex));
			usleep(duration);
			pthread_mutex_lock(&(dongle->dongle_mutex));
		}
	}
	if (!sim_check(sim))
		return (pthread_mutex_unlock(&(dongle->dongle_mutex)) + 1);
	dongle->held = TRUE;
	dongle->queue[0].coder_id = dongle->queue[1].coder_id;
	dongle->queue[0].burnout_time = dongle->queue[1].burnout_time;
	dongle->queue[0].created_at = dongle->queue[1].created_at;
	dongle->queue[1].coder_id = -1;
	pthread_mutex_unlock(&(dongle->dongle_mutex));
	return (0);
}

static void	fill_queue_infos(
	t_sim *sim, int coder_id, t_ticket *ticket, size_t actual_time
)
{
	ticket->coder_id = coder_id;
	ticket->burnout_time = actual_time + sim->config.time_to_burnout;
	ticket->created_at = actual_time;
}

int	lock_dongle(
	t_sim *sim, int coder_id, t_dongle *dongle, size_t last_compile_time
)
{
	t_ticket	*queue;

	pthread_mutex_lock(&(dongle->dongle_mutex));
	queue = dongle->queue;
	if (queue[0].coder_id == -1)
		fill_queue_infos(sim, coder_id, &(queue[0]), last_compile_time);
	else if (sim->config.scheduler == FIFO)
		fill_queue_infos(sim, coder_id, &(queue[1]), last_compile_time);
	else
	{
		if (
			(last_compile_time + sim->config.time_to_burnout)
			> queue[0].burnout_time
		)
			fill_queue_infos(sim, coder_id, &(queue[1]), last_compile_time);
		else
		{
			fill_queue_infos(
				sim, queue[0].coder_id, &(queue[1]),
				queue[0].burnout_time - sim->config.time_to_burnout);
			fill_queue_infos(sim, coder_id, &(queue[0]), last_compile_time);
		}
	}
	return (wait_dongle(sim, coder_id, dongle));
}

t_dongle	*init_dongles(size_t count)
{
	t_dongle	*dongles;
	size_t		actual_time;
	size_t		i;

	dongles = malloc(sizeof(t_dongle) * count);
	if (!dongles)
		return (NULL);
	i = 0;
	actual_time = get_actual_time();
	while (i < count)
	{
		if (pthread_mutex_init(&(dongles[i].dongle_mutex), NULL))
			return (exit_dongle_init(dongles, i, i));
		if (pthread_cond_init(&(dongles[i].cond), NULL))
			return (exit_dongle_init(dongles, i + 1, i));
		dongles[i].held = FALSE;
		dongles[i].available_at = actual_time;
		dongles[i].queue[0].coder_id = -1;
		dongles[i].queue[1].coder_id = -1;
		i++;
	}
	return (dongles);
}
