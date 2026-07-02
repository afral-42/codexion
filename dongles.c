/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:44:18 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 17:53:02 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "threads.h"
#include "dongles.h"
#include "utils.h"

t_dongle    *init_dongles(size_t count)
{
    t_dongle    *dongles;
    size_t      actual_time;
    size_t      i;

    dongles = malloc(sizeof(t_dongle) * count);
    if (!dongles)
        return (NULL);
    i = 0;
    if (get_actual_time(&actual_time)) {
        free(dongles);
        return (NULL);
    }
    while (i < count) {
        pthread_mutex_init(&(dongles[i].dongle_mutex), NULL);
        dongles[i].available_at = actual_time;
        i++;
    }
    return dongles;
}
