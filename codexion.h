/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounoua <abounoua@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 13:05:07 by abounoua          #+#    #+#             */
/*   Updated: 2026/07/22 22:51:41 by abounoua         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_ticket
{
	int		coder_id;
	size_t	created_at;
	size_t	burnout_time;
}	t_ticket;

typedef struct s_dongle
{
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	cond;
	t_ticket		queue[2];
	t_bool			held;
	size_t			available_at;
}	t_dongle;

typedef struct s_config
{
	size_t		number_of_coders;
	size_t		time_to_burnout;
	size_t		time_to_compile;
	size_t		time_to_debug;
	size_t		time_to_refactor;
	size_t		number_of_compiles_required;
	size_t		dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_sim
{
	t_config		config;
	size_t			start_time;
	t_dongle		*dongles;
	pthread_mutex_t	status_mutex;
	pthread_mutex_t	running_mutex;
	t_bool			running;
}	t_sim;

typedef struct s_coders_args
{
	int				id;
	t_bool			end;
	pthread_mutex_t	end_mutex;
	t_sim			*sim;
	size_t			last_compilation;
	pthread_mutex_t	last_compil_mutex;
}	t_coders_args;

typedef struct s_monitor_args
{
	t_coders_args	*coder_args;
	t_sim			*sim;
}	t_monitor_args;

size_t			get_actual_time(void);
int				sim_check(t_sim *sim);
void			*coder_routine(void *args);
t_coders_args	*generate_args(t_sim *sim);
t_dongle		*init_dongles(size_t count);
size_t			last_ct(t_coders_args *args);
void			*monitor_routine(void *args);
void			*exit_dongle_init(t_dongle *dongles, size_t dongle_len, size_t cond_len);
void			clean_simulation_mutexes(t_sim *sim);
void			wait_threads(pthread_t *threads, size_t size);
pthread_t		*init_threads(t_sim *sim, t_coders_args *args);
int				parse_params(int ac, char **av, t_config *config);
void			clean_dongle_conds(t_dongle *dongles, size_t size);
void			clean_dongle_mutexes(t_dongle *dongles, size_t size);
void			clean_coders_mutexes(t_coders_args *args, size_t end_size, size_t compil_size);
int				lock_dongle(t_sim *sim, int coder_id, t_dongle *dongle, size_t last_compile_time);
int				is_number(char *s);
void			print_usage(void);
int				validate_numeric_args(char **av, size_t start, size_t end);
void			print_status(
					t_coders_args *coder_args, char *status, size_t waiting);


#endif