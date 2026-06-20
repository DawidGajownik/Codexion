/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:09:21 by dgajowni          #+#    #+#             */
/*   Updated: 2026/06/20 19:09:08 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	timeval_to_timestamp(t_params *params, t_coder *coder)
{
	return (coder->last_compile_time.tv_sec - params->time.tv_sec) * 1000
		+ (coder->last_compile_time.tv_usec - params->time.tv_usec) / 1000;
}

void	maybe_wait(t_params *params, long time_now, int left, int right, t_coder *coder)
{
	long	diff_left;
	long	diff_right;
	long	time_to_sleep;
	long	last_comp_time;

	last_comp_time = timeval_to_timestamp(params, coder);
	last_comp_time = last_comp_time;

	// printf(
	// 	"\n coder %d lastcompile = %ld\ntimenow = %ld \nmutex_time_left = %ld \nmutex_time_right = %ld \ndongle cooldown = %ld\n",
	// 	coder->id,
	// 	last_comp_time,
	// 	time_now,
	// 	params->mutex_time[left],
	// 	params->mutex_time[right],
	// 	params->dongle_cooldown
	// );

	diff_left = time_now - params->mutex_time[left];
	diff_right = time_now - params->mutex_time[right];
	time_to_sleep = 0;
	if (diff_left > diff_right)
	{
		if (params->dongle_cooldown > diff_left)
			time_to_sleep = params->dongle_cooldown - diff_left;
	}
	else
	{
		if (params->dongle_cooldown > diff_right)
			time_to_sleep = params->dongle_cooldown - diff_right;
	}
	//printf("time to sleep = %ld\n\n", time_to_sleep);
	usleep(time_to_sleep * 1000);
}

void print_all_coders(t_params *params)
{
	t_coder *coders = params->coders;
	long smallest_lct;
	long last_comp_time;
	t_coder *coder_edf;

	smallest_lct = LONG_MAX;
	int i;
	coder_edf = coders;
	i = -1;
	while (++i < params->number_of_coders)
	{
		last_comp_time = timeval_to_timestamp(params, coders+i);
		if (last_comp_time < smallest_lct)
		{
			smallest_lct = last_comp_time;
			coder_edf = coders+i;
		}
		printf("Coder %d, place = %d, last_compile = %ld\n", (coders+i)->id, (coders+i)->place, last_comp_time);
	}
	printf("EDF Coder %d, place = %d,last_compile = %ld\n", coder_edf->id, coder_edf->place, smallest_lct);
}

void	lock_mutexes(t_params *params, t_coder *coder)
{
	int				left;
	int				right;
	long 			time_now;
	struct timeval 	tv;

	if (params->burned_out)
		return;
	left = coder->id - 1;
	right = coder->id % params->number_of_coders;
	time_now = get_timestamp(params, &tv);
	if (burned_out(coder, params) == 1)
		return unlock_mutexes(params, coder->id);
	//print_all_coders(params);
	maybe_wait(params, time_now, left, right, coder);
	if (coder->id % 2 != 0)
	{
		pthread_mutex_lock(&params->mutex[left]);
		printfm(params, "%ld %d has taken a dongle\n", coder);
		pthread_mutex_lock(&params->mutex[right]);
		printfm(params, "%ld %d has taken a dongle\n", coder);
	}
	else
	{
		usleep(10);
		pthread_mutex_lock(&params->mutex[right]);
		printfm(params, "%ld %d has taken a dongle\n", coder);
		pthread_mutex_lock(&params->mutex[left]);
		printfm(params, "%ld %d has taken a dongle\n", coder);
	}
}

void	unlock_mutexes(t_params *params, int id)
{
	int	left;
	int	right;
	long time_now;
	struct timeval tv;

	left = id - 1;
	right = id % params->number_of_coders;
	time_now = get_timestamp(params, &tv);
	if (id % 2 != 0)
	{
		params->mutex_time[right] = time_now;
		pthread_mutex_unlock(&params->mutex[right]);
		params->mutex_time[left] = time_now;
		pthread_mutex_unlock(&params->mutex[left]);
	}
	else
	{
		params->mutex_time[left] = time_now;
		pthread_mutex_unlock(&params->mutex[left]);
		params->mutex_time[right] = time_now;
		pthread_mutex_unlock(&params->mutex[right]);
	}
}

int	init_mutexes(t_params *params)
{
	int	i;

	i = 0;
	params->mutex = malloc(sizeof(pthread_mutex_t) * params->number_of_coders);
	if (params->mutex == NULL)
		return (-1);
	params->mutex_time = malloc(sizeof(long) * params->number_of_coders);
	if (params->mutex_time == NULL)
		return (-1);
	while (i < params->number_of_coders)
	{
		params->mutex_time[i] = INT_MIN;
		pthread_mutex_init(&(params->mutex[i++]), NULL);
	}
	pthread_mutex_init(&(params->mutex_burned_out), NULL);
	pthread_mutex_init(&(params->print_mutex), NULL);
	pthread_mutex_init(&(params->places_mutex), NULL);
	return (0);
}

int	destroy_mutexes(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->number_of_coders)
	{
		pthread_mutex_destroy(&(params->mutex[i++]));
	}
	free((params->mutex_time));
	return (0);
}
