/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:21:00 by dgajowni          #+#    #+#             */
/*   Updated: 2026/02/10 18:39:35 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_timestamp(t_params *params, struct timeval *tv)
{
	long			timestamp;

	gettimeofday(tv, NULL);
	timestamp = ((*tv).tv_sec - params->time.tv_sec) * 1000000L
		+ ((*tv).tv_usec - params->time.tv_usec);
	return (timestamp);
}
