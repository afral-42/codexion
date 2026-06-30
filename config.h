/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:00 by abounoua          #+#    #+#             */
/*   Updated: 2026/06/30 15:24:01 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

typedef enum e_scheduler {
    FIFO,
    EDF
} t_scheduler;

typedef struct s_config {
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    t_scheduler scheduler;
} t_config;

#endif
