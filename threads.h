#ifndef THREADS_H
# define THREADS_H

# include <pthread.h>
# include "config.h"

pthread_t   *init_threads(t_config *config);
void    wait_threads(pthread_t *threads, int size);

#endif