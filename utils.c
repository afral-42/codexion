/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 17:10:01 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/01 17:14:45 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include <sys/time.h>
#include <stddef.h>

size_t  get_actual_time(int *flag)
{
    struct timeval  actual_time;

    if (gettimeofday(&actual_time, NULL) == -1)
    {
        *flag = 1;
        return (0);
    }
    return (actual_time.tv_sec * 1000 + actual_time.tv_usec);
}
