/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:09:21 by dgajowni          #+#    #+#             */
/*   Updated: 2026/05/16 21:54:48 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_mutexes(t_params *params, t_coder *coder)
{
	int				left;
	int				right;

	left = coder->id - 1;
	right = coder->id % params->number_of_coders;
	if (coder->id % 2 != 0)
	{
		pthread_mutex_lock(&params->mutex[left]);
		printfm(*params, "%ld %d has taken a dongle\n", coder);
		pthread_mutex_lock(&params->mutex[right]);
		printfm(*params, "%ld %d has taken a dongle\n", coder);
	}
	else
	{
		pthread_mutex_lock(&params->mutex[right]);
		printfm(*params, "%ld %d has taken a dongle\n", coder);
		pthread_mutex_lock(&params->mutex[left]);
		printfm(*params, "%ld %d has taken a dongle\n", coder);
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
