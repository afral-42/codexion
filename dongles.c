/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:44:18 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 16:37:34 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "threads.h"
#include "dongles.h"
#include "utils.h"

t_dongle    *init_dongles(size_t count)
{
    t_dongle    *dongles;
    size_t      i;

    dongles = malloc(sizeof(t_dongle) * count);
    if (!dongles)
        return (NULL);
    i = 0;
    while (i < count) {
        pthread_mutex_init(&(dongles[i].dongle_mutex), NULL);
        if (get_actual_time(&(dongles[i].available_at))) {
            free(dongles);
            return (NULL);
        }
        i++;
    }
    return dongles;
}
