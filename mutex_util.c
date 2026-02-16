/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:09:21 by dgajowni          #+#    #+#             */
/*   Updated: 2026/02/12 14:27:27 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_mutexes(t_params *params, int id)
{
	int				left;
	int				right;
	long			timestamp;
	struct timeval	tv;
	static int		first;

	first = 0;
	left = id - 1;
	right = id % params->number_of_coders;
	timestamp = get_timestamp(params, &tv);
	if (id % 2 != 0)
	{
		pthread_mutex_lock(&params->mutex[left]);
		printfm(*params, "%ld %d has taken a dongle l1\n", timestamp, id);
		pthread_mutex_lock(&params->mutex[right]);
		printfm(*params, "%ld %d has taken a dongle l2\n", timestamp, id);
	}
	else
	{
		if (first == 0)
			usleep(10);
		first = 1;
		pthread_mutex_lock(&params->mutex[right]);
		printfm(*params, "%ld %d has taken a dongle r1\n", timestamp, id);
		pthread_mutex_lock(&params->mutex[left]);
		printfm(*params, "%ld %d has taken a dongle r2\n", timestamp, id);
	}
}

void	unlock_mutexes(t_params *params, int id)
{
	int	left;
	int	right;

	left = id - 1;
	right = id % params->number_of_coders;
	if (id % 2 != 0)
	{
		pthread_mutex_unlock(&params->mutex[right]);
		pthread_mutex_unlock(&params->mutex[left]);
	}
	else
	{
		pthread_mutex_unlock(&params->mutex[left]);
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
	while (i < params->number_of_coders)
		pthread_mutex_init(&(params->mutex[i++]), NULL);
	pthread_mutex_init(&(params->print_mutex), NULL);
	return (0);
}

int	destroy_mutexes(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->number_of_coders)
		pthread_mutex_destroy(&(params->mutex[i++]));
	return (0);
}
