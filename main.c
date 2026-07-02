/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anselme <anselme@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 15:24:03 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/02 17:02:34 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include "threads.h"
#include "dongles.h"
#include "coders.h"

void print_config(t_config *config)
{
    if (!config)
        return;

    printf("=== ÉTAT DE LA CONFIGURATION ===\n");
    printf("Nombre de codeurs   : %zu\n", config->number_of_coders);
    printf("Temps avant burnout : %zu ms\n", config->time_to_burnout);
    printf("Temps de compilo    : %zu ms\n", config->time_to_compile);
    printf("Temps de debug      : %zu ms\n", config->time_to_debug);
    printf("Temps de refacto    : %zu ms\n", config->time_to_refactor);
    printf("Compilations requises : %zu\n", config->number_of_compiles_required);
    printf("Cooldown du dongle  : %zu ms\n", config->dongle_cooldown);
    printf("Scheduler           : %d\n", config->scheduler);
    printf("================================\n");
}

int exit_init(t_config *config, t_coders_args *args, pthread_t *threads)
{
    free(config);
    free(threads);
    return (1);
}

int main(int ac, char **av)
{
    t_config        *config;
    t_coders_args   *args;
    pthread_t       *threads;
    t_dongle        *dongles;

    config = parse_params(ac, av);
    if (!config)
        return (exit_init(NULL, NULL, NULL));
    print_config(config);
    args = generate_args(config);
    if (!args)
        return (exit_init(config, NULL, NULL));
    threads = init_threads(config, args);
    if (!threads)
        return (exit_init(config, args, NULL));
    dongles = init_dongles(config->number_of_coders);
    if (!dongles)
        return (exit_init(config, args, threads));
    wait_threads(threads, config->number_of_coders);
    free(config);
    free(threads);
    free(args);
    free(dongles);
}
