/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:44:18 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/23 17:01:59 by abounoua         ###   ########lyon.fr   */
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

int	wait_dongle(t_sim *sim, int coder_id, t_dongle *dongle)
{
	if (!check_disponibility(coder_id, dongle) && sim_check(sim))
    {
        pthread_mutex_unlock(&(dongle->dongle_mutex));
        return (1);
    }
	dongle->held = TRUE;
    unsubscribe_to_queue(dongle);
	pthread_mutex_unlock(&(dongle->dongle_mutex));
	return (0);
}

int try_lock_dongles(t_dongle *first, t_dongle *second, t_thread_args *args)
{
    int flag;

    while (1)
    {
        if (!sim_check(args->sim))
            return (1);
        pthread_mutex_lock(&(first->dongle_mutex));
        flag = wait_dongle(args->sim, args->id, first);
        if (flag)
        {
		    usleep(1000);
            continue;
        }
        pthread_mutex_lock(&(second->dongle_mutex));
        flag = wait_dongle(args->sim, args->id, second);
        if (flag)
        {
            release_single_dongle(args->sim, first, 0, FALSE);
            subscribe_dongle(args->sim, args->id, first, last_ct(args));
		    usleep(1000);
            continue;
        }
        break;
    }
    return (0);
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
			return (exit_dongle_init(dongles, i));
		dongles[i].held = FALSE;
		dongles[i].available_at = actual_time;
		dongles[i].queue[0].coder_id = -1;
		dongles[i].queue[1].coder_id = -1;
		i++;
	}
	return (dongles);
}
