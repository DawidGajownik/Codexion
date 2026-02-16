/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:15:05 by dgajowni          #+#    #+#             */
/*   Updated: 2026/02/16 14:20:57 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder, t_params *params)
{
	struct timeval	tv;
	long			timestamp;
	
	lock_mutexes(params, coder->id);
	timestamp = get_timestamp(params, &tv);
	printfm(*params, "%ld %d is compiling...\n", timestamp, coder->id);
	usleep(params->time_to_compile*1000);
	coder->compiles_done++;
	coder->last_compile_time = tv;
	unlock_mutexes(params, coder->id);
}

void	debug(t_coder *coder, t_params *params)
{
	struct timeval	tv;
	long			timestamp;

	timestamp = get_timestamp(params, &tv);
	printfm(*params, "%ld %d is debugging...\n", timestamp, coder->id);
	usleep(params->time_to_debug*1000);
}

void	refactor(t_coder *coder, t_params *params)
{
	struct timeval	tv;
	long			timestamp;

	timestamp = get_timestamp(params, &tv);
	printfm(*params, "%ld %d is refractoring...\n", timestamp, coder->id);
	usleep(params->time_to_refactor*1000);
}

int	burned_out(t_coder *coder, t_params *params)
{
	struct timeval	tv;
	long			timestamp;
	long			coder_timestamp;

	if (coder->last_compile_time.tv_sec == 0)
		return(0);
	timestamp = get_timestamp(params, &tv);
	coder_timestamp = (coder->last_compile_time.tv_sec - params->time.tv_sec) * 1000000L
		+ (coder->last_compile_time.tv_usec - params->time.tv_usec);
	if (timestamp - coder_timestamp >= params->time_to_burnout)
	{
		printfm(*params, "%ld %d is burned_out...\n", timestamp, coder->id);
		return(1);
	}
	return(0);
}

void	*coder_thread(void *arg)
{
	t_coder			*coder;
	t_params		*params;

	coder = (t_coder *)arg;
	params = coder->params;
	while (coder->compiles_done < params->number_of_compiles_required
			&& burned_out(coder, params) == 0)
	{
		compile(coder, params);
		debug(coder, params);
		refactor(coder, params);
	}
	return (NULL);
}

int	init_coders(t_params *params)
{
	int	i;

	i = -1;
	params->coders = malloc(sizeof(t_coder) * params->number_of_coders);
	if (params->coders == NULL)
		return (-1);
	while (++i < params->number_of_coders)
	{
		params->coders[i].id = i + 1;
		params->coders[i].compiles_done = 0;
		params->coders[i].last_compile_time.tv_sec = 0;
		params->coders[i].last_compile_time.tv_usec = 0;
		params->coders[i].thread = 0;
		params->coders[i].params = params;
	}
	return (0);
}
