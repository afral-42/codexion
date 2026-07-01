#ifndef DONGLES_H
# define DONGLES_H

# include <pthread.h>

typedef struct s_dongle {
    pthread_mutex_t dongle_mutex;
    size_t          available_at;
} t_dongle;


t_dongle    *init_dongles(int count);

#endif
