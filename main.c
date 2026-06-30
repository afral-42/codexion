/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:03 by abounoua          #+#    #+#             */
/*   Updated: 2026/06/30 15:24:18 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "parsing.h"
#include <stdio.h>

void print_config(t_config *config)
{
    if (!config)
        return;

    printf("=== ÉTAT DE LA CONFIGURATION ===\n");
    printf("Nombre de codeurs   : %d\n", config->number_of_coders);
    printf("Temps avant burnout : %d ms\n", config->time_to_burnout);
    printf("Temps de compilo    : %d ms\n", config->time_to_compile);
    printf("Temps de debug      : %d ms\n", config->time_to_debug);
    printf("Temps de refacto    : %d ms\n", config->time_to_refactor);
    printf("Compilations requises : %d\n", config->number_of_compiles_required);
    printf("Cooldown du dongle  : %d ms\n", config->dongle_cooldown);
    printf("Scheduler           : %d\n", config->scheduler);
    printf("================================\n");
}

int main(int ac, char **av) {
    t_config *config;

    config = parse_params(ac, av);
    if (!config)
        return (1);
    print_config(config);
    free(config);
}
