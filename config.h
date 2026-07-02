/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:00 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 15:55:44 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <stdlib.h>

typedef enum e_scheduler {
    FIFO,
    EDF
} t_scheduler;

typedef struct s_config {
    size_t      number_of_coders;
    size_t      time_to_burnout;
    size_t      time_to_compile;
    size_t      time_to_debug;
    size_t      time_to_refactor;
    size_t      number_of_compiles_required;
    size_t      dongle_cooldown;
    t_scheduler scheduler;
} t_config;

#endif
