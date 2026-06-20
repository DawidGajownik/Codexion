/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:47:38 by dgajowni          #+#    #+#             */
/*   Updated: 2026/06/20 18:31:14 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

typedef enum c_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

struct	s_params;

typedef struct s_coder
{
	int				id;
	int				compiles_done;
	pthread_t		thread;
	struct timeval	last_compile_time;
	struct s_params	*params;
	int				place;
}t_coder;


// typedef struct s_heap
// {
// 	int 	**data;
// 	int		id;
// }t_heap;


// typedef struct s_dongle
// {
// 	int 	id;
// 	int in_use;
// 	long avilable_at;

// 	pthread_mutex_t	mutex;
// 	pthread_cond_t	cond;
// 	t_heap queque;
// 	struct s_sim sim;

// }t_dongle;

// typedef struct s_sim
// {
// 	t_params params;
// 	long start_time;
	
// }t_sim;



typedef struct s_params
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	struct timeval	time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	mutex_burned_out;
	pthread_mutex_t	places_mutex;
	long			*mutex_time;
	t_scheduler		scheduler;
	t_coder			*coders;
	bool			burned_out;
}	t_params;

int		parse_arguments(int argc, char *argv[], t_params *params);
int		print_error(char *program_name);
int		print_arguments(t_params *params);
void	lock_mutexes(t_params *params, t_coder *coder);
void	unlock_mutexes(t_params *params, int id);
int		init_mutexes(t_params *params);
int		destroy_mutexes(t_params *params);
void	*coder_thread(void *arg);
int		init_coders(t_params *params);
int		printfm(t_params *params, const char *str, t_coder *coder);
long	get_timestamp(t_params *params, struct timeval *tv);
int		burned_out(t_coder *coder, t_params *params);

#endif