/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:42:42 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 17:13:02 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "config.h"
#include "coders.h"
#include "utils.h"

void    *exit_threads_init(pthread_t *threads, t_coders_args *configs)
{
    free(threads);
    free(configs);
    return (NULL);
}

int fill_args(t_config *config, t_coders_args *coders_args, pthread_mutex_t *status_mutex)
{
    size_t  i;

    i = 0;
    while (i < config->number_of_coders) {
        coders_args[i].id = i + 1;
        coders_args[i].config = config;
        if (get_actual_time(&(coders_args[i].start_time)))
        {
            free(coders_args);
            free(status_mutex);
            return (1);
        }
        coders_args[i].status_mutex = status_mutex;
        i++;
    }
    return (0);
}
t_coders_args   *generate_args(t_config *config)
{
    t_coders_args	*coders_args;
    pthread_mutex_t *status_mutex;

    coders_args = malloc(sizeof(t_coders_args) * config->number_of_coders);
    if (!coders_args)
        return (NULL);
    status_mutex = malloc(sizeof(pthread_mutex_t));
    if (!status_mutex)
    {
        free(coders_args);
        return (NULL);
    }
    if (pthread_mutex_init(status_mutex, NULL))
    {
        free(status_mutex);
        free(coders_args);
        return (NULL);
    }
    if (fill_args(config, coders_args, status_mutex))
    {
        free(status_mutex);
        free(coders_args);
    }
    return coders_args;
}


pthread_t   *init_threads(t_config *config, t_coders_args *args)
{
    size_t			i;
    pthread_t       *threads;

    threads = malloc(sizeof(pthread_t) * config->number_of_coders);
    if (!threads)
        return (exit_threads_init(NULL, NULL));
    i = 0;
    while (i < config->number_of_coders) {
        if (pthread_create(
            &(threads[i]), NULL, coder_function, &args[i]
        ))
            return (exit_threads_init(threads, args));
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
