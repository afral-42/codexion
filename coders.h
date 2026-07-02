#ifndef CODERS_H
# define CODERS_H

# include "config.h"
# include <pthread.h>

typedef struct s_coders_args {
    t_config        *config;
    pthread_mutex_t *status_mutex;
    size_t          start_time;

    int         id;
}   t_coders_args;

void    *coder_function(void *args);

#endif
