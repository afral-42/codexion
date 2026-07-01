#include <stdio.h>
#include "coders.h"


void    *coder_function(void *args)
{
    t_coders_args *config;

    config = (t_coders_args *)args;

    printf("Numéro %d: Compilation\n", config->id);
    printf("Numéro %d: Debug\n", config->id);
    printf("Numéro %d: Refacto\n", config->id);

    return NULL;
}
