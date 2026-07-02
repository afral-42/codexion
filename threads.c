/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:42:42 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 18:32:29 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "config.h"
#include "coders.h"
#include "utils.h"
#include "codexion.h"

void    *exit_threads_init(pthread_t *threads)
{
    free(threads);
    return (NULL);
}

void    fill_args(t_sim *sim, t_coders_args *coders_args)
{
    size_t  i;
    size_t  start_time;

    i = 0;
    while (i < sim->config.number_of_coders)
    {
        coders_args[i].id = i + 1;
        coders_args[i].sim = sim;
        i++;
    }
}
t_coders_args   *generate_args(t_sim *sim)
{
    t_coders_args	*coders_args;

    coders_args = malloc(sizeof(t_coders_args) * sim->config.number_of_coders);
    if (!coders_args)
        return (NULL);
    if (pthread_mutex_init(&(sim->status_mutex), NULL))
    {
        free(coders_args);
        return (NULL);
    }
    fill_args(sim, coders_args);
    return coders_args;
}


pthread_t   *init_threads(t_sim *sim, t_coders_args *args)
{
    size_t			i;
    pthread_t       *threads;

    threads = malloc(sizeof(pthread_t) * sim->config.number_of_coders);
    if (!threads)
        return (exit_threads_init(NULL));
    i = 0;
    if (get_actual_time((&sim->start_time)))
        return (exit_threads_init(threads));
    while (i < sim->config.number_of_coders) {
        if (pthread_create(
            &(threads[i]), NULL, coder_function, &args[i]
        ))
            return (exit_threads_init(threads));
        i++;
    }
    return threads;
}

void    wait_threads(pthread_t *threads, size_t size)
{
    size_t	i;

    i = 0;
    while (i < size) {
        pthread_join(threads[i], NULL);
        i++;
    }
}
