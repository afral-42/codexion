/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:57:31 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/23 17:25:31 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    unsubscribe_to_queue(t_dongle *dongle)
{
    dongle->queue[0].coder_id = dongle->queue[1].coder_id;
    dongle->queue[0].burnout_time = dongle->queue[1].burnout_time;
    dongle->queue[0].created_at = dongle->queue[1].created_at;
    dongle->queue[1].coder_id = -1;
}


static void	fill_queue_infos(
	t_sim *sim, int coder_id, t_ticket *ticket, size_t actual_time
)
{
	ticket->coder_id = coder_id;
	ticket->burnout_time = actual_time + sim->config.time_to_burnout;
	ticket->created_at = actual_time;
}

void    subscribe_dongle(
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
    pthread_mutex_unlock(&(dongle->dongle_mutex));
}

void	release_single_dongle(
	t_sim *sim, t_dongle *dongle, size_t time, t_bool cooldown
)
{
	pthread_mutex_lock(&(dongle->dongle_mutex));
	if (cooldown)
        dongle->available_at = time + sim->config.dongle_cooldown;
	dongle->held = FALSE;
	pthread_mutex_unlock(&(dongle->dongle_mutex));
}