#ifndef CODERS_H
# define CODERS_H

# include "config.h"

typedef struct s_coders_args {
    t_config    *config;
    int         id;
}   t_coders_args;

void    *coder_function(void *args);

#endif
