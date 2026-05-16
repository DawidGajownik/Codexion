/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:21:00 by dgajowni          #+#    #+#             */
/*   Updated: 2026/05/16 20:23:06 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_timestamp(t_params *params, struct timeval *tv)
{
	long			timestamp;

	gettimeofday(tv, NULL);
	timestamp = ((*tv).tv_sec - params->time.tv_sec) * 1000
		+ ((*tv).tv_usec - params->time.tv_usec) / 1000;
	return (timestamp);
}
