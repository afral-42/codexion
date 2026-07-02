#ifndef CODERS_H
# define CODERS_H

# include "config.h"
# include <pthread.h>
# include "codexion.h"

typedef struct s_coders_args {
    int		id;
    t_sim   *sim;
}   t_coders_args;

void    *coder_function(void *args);

#endif
