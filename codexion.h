#ifndef CODEXION_H
# define CODEXION_H

# include "config.h"
# include <pthread.h>

typedef struct s_sim {
    t_config        config;
    size_t          start_time;
    pthread_mutex_t status_mutex;
} t_sim;

#endif