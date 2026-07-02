/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 20:07:39 by anselme           #+#    #+#             */
/*   Updated: 2026/07/02 17:16:29 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "utils.h"
#include "coders.h"

int print_status(t_coders_args *coder_args, char *status)
{
    size_t  sim_time;
    size_t  actual_time;

    pthread_mutex_lock(coder_args->status_mutex);
    if (get_actual_time(&actual_time))
    {
        pthread_mutex_unlock(coder_args->status_mutex);
        return (1);
    }
    sim_time = actual_time - coder_args->start_time;
    printf("%zu %d %s\n", sim_time, coder_args->id, status);
    pthread_mutex_unlock(coder_args->status_mutex);

    return (0);
}

void    *coder_function(void *args)
{
    t_coders_args	*coder_args;
    t_config		*config;
    size_t          i;

    coder_args = (t_coders_args *)args;
    config = coder_args->config;
    i = 0;
    while (i < 1000) {
        print_status(coder_args, "has taken a dongle");
        print_status(coder_args, "has taken a dongle");
        print_status(coder_args, "is compiling");
        print_status(coder_args, "is debugging");
        print_status(coder_args, "is refactoring");
        i++;
    }

    return (NULL);
}
