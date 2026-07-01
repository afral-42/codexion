/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 18:44:18 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/01 17:00:26 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <threads.h>
#include "dongles.h"

t_dongle    *init_dongles(int count)
{
    t_dongle    *dongles;
    size_t      i;

    dongles = malloc(sizeof(t_dongle) * count);
    if (!dongles)
        return (NULL);
    i = 0;
    while (i < count) {
        pthread_mutex_init(&(dongles[i].dongle_mutex), NULL);   
        i++;
    }

    return dongles;
}
