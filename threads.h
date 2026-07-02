#ifndef THREADS_H
# define THREADS_H

# include <pthread.h>
# include "config.h"
# include "coders.h"

pthread_t   *init_threads(t_config *config, t_coders_args *args);
void        wait_threads(pthread_t *threads, size_t size);
t_coders_args   *generate_args(t_config *config);

#endif
