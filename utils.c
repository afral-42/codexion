/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:10:01 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 17:18:02 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <sys/time.h>
#include <stddef.h>

int	get_actual_time(size_t *time)
{
    struct timeval  actual_time;

    if (gettimeofday(&actual_time, NULL) == -1)
        return (1);
    *time = ((actual_time.tv_sec * 1000) + (actual_time.tv_usec / 1000));
    return (0);
}
