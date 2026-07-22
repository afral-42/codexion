/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 20:07:39 by anselme           #+#    #+#             */
/*   Updated: 2026/07/22 18:01:48 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "codexion.h"


static void    release_dongles(t_sim *sim, size_t coder_id)
{
    t_dongle    *left;
    t_dongle    *right;
    size_t      actual_time;

    actual_time = get_actual_time();
    left = &(sim->dongles[coder_id]);
    right = &(sim->dongles[(coder_id + 1) % sim->config.number_of_coders]);
    pthread_mutex_lock(&(left->dongle_mutex));
    left->available_at = actual_time + sim->config.dongle_cooldown;
    left->held = FALSE;
    pthread_cond_broadcast(&(left->cond));
    pthread_mutex_unlock(&(left->dongle_mutex));
    pthread_mutex_lock(&(right->dongle_mutex));
    right->available_at = actual_time + sim->config.dongle_cooldown;
    right->held = FALSE;
    pthread_cond_broadcast(&(right->cond));
    pthread_mutex_unlock(&(right->dongle_mutex));
}

int get_dongles(t_sim *sim, int coder_id)
{
    t_dongle    *left;
    t_dongle    *right;

    left = &(sim->dongles[coder_id]);
    right = &(sim->dongles[(coder_id + 1) % sim->config.number_of_coders]);
    if (coder_id % 2 == 0)
    {
        if (lock_dongle(sim, coder_id, left) || lock_dongle(sim, coder_id, right))
            return (1);
    }
    else
    {
        if (lock_dongle(sim, coder_id, right) || lock_dongle(sim, coder_id, left))
            return (1);
    }
    return (0);
}

void    coder_actions(t_sim *sim, t_coders_args *coder_args)
{
    print_status(coder_args, "has taken a dongle", 0);
    print_status(coder_args, "has taken a dongle", 0);
    print_status(coder_args, "is compiling", sim->config.time_to_compile);
    release_dongles(sim, coder_args->id);
    print_status(coder_args, "is debugging", sim->config.time_to_debug);
    print_status(coder_args, "is refactoring", sim->config.time_to_refactor);
}

void    *coder_routine(void *args)
{
    t_coders_args	*coder_args;
    t_config		*config;
    t_sim           *sim;
    size_t          compilations;
    
    compilations = 0;
    coder_args = (t_coders_args *)args;
    config = &(coder_args->sim->config);
    sim = coder_args->sim;
    pthread_mutex_lock(&(sim->running_mutex));
    while (compilations < config->number_of_compiles_required && sim->running)
    {
        pthread_mutex_unlock(&(sim->running_mutex));
        if (get_dongles(sim, coder_args->id))
            return (NULL);
        compilations++;
    }
    pthread_mutex_lock(&(coder_args->end_mutex));
    coder_args->end = TRUE;
    pthread_mutex_unlock(&(coder_args->end_mutex));
    return (NULL);
}
