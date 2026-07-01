/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:42:42 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/01 16:51:05 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "config.h"
#include "coders.h"


static t_coders_args   *generate_configs(t_config *config)
{
    size_t			i;
    t_coders_args	*configs;

    configs = malloc(sizeof(t_coders_args) * config->number_of_coders);
    if (!configs)
        return (NULL);
    i = 0;
    while (i < config->number_of_coders) {
        configs[i].id = i + 1;
        configs[i].config = config;
        i++;
    }
    return configs;
}


pthread_t   *init_threads(t_config *config)
{
    size_t			i;
    pthread_t       *threads;
    t_coders_args   *configs;

    threads = malloc(sizeof(pthread_t) * config->number_of_coders);
    if (!threads)
        return NULL;
    configs = generate_configs(config);
    if (!configs) {
        free(threads);
        return NULL;
    }
    i = 0;
    while (i < config->number_of_coders) {
        if (pthread_create(
            &(threads[i]), NULL, coder_function, &configs[i]
        )) {
            free(threads);
            free(configs);
            return (NULL);
        }
        i++;
    }
    return threads;
}

void    wait_threads(pthread_t *threads, int size)
{
    size_t	i;

    i = 0;
    while (i < size) {
        pthread_join(threads[i], NULL);
        i++;
    }
}
