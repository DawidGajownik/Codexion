/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:47:38 by dgajowni          #+#    #+#             */
/*   Updated: 2026/02/10 18:42:07 by dgajowni         ###   ########.fr       */
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
}	t_coder;

typedef struct s_params
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	struct timeval	time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*mutex;
	t_scheduler		scheduler;
	t_coder			*coders;
}	t_params;

int		parse_arguments(int argc, char *argv[], t_params *params);
int		print_error(char *program_name);
int		print_arguments(t_params *params);
void	lock_mutexes(t_params *params, int id);
void	unlock_mutexes(t_params *params, int id);
int		init_mutexes(t_params *params);
int		destroy_mutexes(t_params *params);
void	*coder_thread(void *arg);
int		init_coders(t_params *params);
int		printfm(t_params params, const char *str, long timestamp,
			int id);
long	get_timestamp(t_params *params, struct timeval *tv);

#endif