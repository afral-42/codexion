/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:05 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 15:47:07 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config.h"

void    *exit_parsing(t_config *config, char *err)
{
    printf("Error: %s", err);
    free(config);
    return (NULL);
}

int is_number(char *s)
{
    while (*s != '\0') {
        if (!(*s >= '0' && *s <= '9'))
            return (0);
        s++;
    }
    s++;
    return (1);
}

void    parse_config_numbers(t_config *config, char **av)
{
    config->number_of_coders = atoi(av[1]);
    config->time_to_burnout = atoi(av[2]);
    config->time_to_compile = atoi(av[3]);
    config->time_to_debug = atoi(av[4]);
    config->time_to_refactor = atoi(av[5]);
    config->number_of_compiles_required = atoi(av[6]);
    config->dongle_cooldown = atoi(av[7]);
}

int parse_config_scheduler(t_config *config, char **av)
{
    if (!strcmp(av[8], "edf"))
        config->scheduler = EDF;
    else if (!strcmp(av[8], "fifo"))
        config->scheduler = FIFO;
    else
        return (1);

    return (0);
}

t_config    *parse_params(int ac, char **av)
{
    size_t  	i;
    int			flag; 
    t_config	*config;

    i = 1;
    config = (t_config *)malloc(sizeof(t_config));
    if (!config)
        return (NULL);
    if (ac != 9)
        return exit_parsing(config, "invalid arguments");
    while (i < 8) {
        if (!is_number(av[i]))
            return exit_parsing(config, "time arguments musts be positive numbers");
        i++;
    }
    parse_config_numbers(config, av);
    if (parse_config_scheduler(config, av))
        return exit_parsing(config, "scheduler must be fifo or edf");
    return config;
}
