/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:09:21 by dgajowni          #+#    #+#             */
/*   Updated: 2026/06/01 17:43:04 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	printf("\ntimenow = %ld mutex_time_left = %ld mutex_time_right = %ld dongle cooldown = %d\n", time_now, params->mutex_time[left], params->mutex_time[right], params->dongle_cooldown);

	if (coder->id % 2 != 0)
	{
		printf("opcja1\n");
		// if (time_now-params->mutex_time[left] < params->dongle_cooldown)
		// {
		// 	printf("1a lef\n");
		// 	printf("skibidi %d %ld %ld\n", params->dongle_cooldown, time_now, params->mutex_time[left]);
		// 	usleep((params->dongle_cooldown - (time_now-params->mutex_time[left])*1000));
		// 	printf("1b lef\n");
		// }
		
		pthread_mutex_lock(&params->mutex[left]);
		printfm(params, "%ld %d has taken a dongle\n", coder);
		// if (time_now-params->mutex_time[right] < params->dongle_cooldown)
		// {
		// 	printf("1a rig\n");
		// 	usleep((params->dongle_cooldown - (time_now-params->mutex_time[right])*1000));
		// 	printf("1b rig\n");
		// }
		pthread_mutex_lock(&params->mutex[right]);
		if (burned_out(coder, params) == 1)
			return unlock_mutexes(params, coder->id);
		printfm(params, "%ld %d has taken a dongle\n", coder);
	}
	else
	{
		printf("opcja2\n");
		usleep(10);
		// if (time_now-params->mutex_time[right] < params->dongle_cooldown)
		// {
		// 	printf("2a rig \n");
		// 	usleep((params->dongle_cooldown - (time_now-params->mutex_time[right]))*1000);
		// 	printf("2b rig\n");
		// }
		pthread_mutex_lock(&params->mutex[right]);
		printfm(params, "%ld %d has taken a dongle\n", coder);
		if (time_now-params->mutex_time[left] < params->dongle_cooldown)
		// {
		// 	printf("2a lef\n");
		// 	usleep((params->dongle_cooldown - (time_now-params->mutex_time[left]))*1000);
		// 	printf("2b lef\n");
		// }
		printf("dalej\n");
		pthread_mutex_lock(&params->mutex[left]);
		if (burned_out(coder, params) == 1)
			return unlock_mutexes(params, coder->id);
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
