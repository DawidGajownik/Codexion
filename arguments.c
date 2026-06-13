/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:47:42 by dgajowni          #+#    #+#             */
/*   Updated: 2026/06/01 21:30:29 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_arguments(int argc, char *argv[], t_params *params)
{
	if (argc < 9 || argc > 9)
		return (-1);
	params->number_of_coders = atoi(argv[1]);
	params->time_to_burnout = atoi(argv[2]);
	params->time_to_compile = atoi(argv[3]);
	params->time_to_debug = atoi(argv[4]);
	params->time_to_refactor = atoi(argv[5]);
	params->number_of_compiles_required = atoi(argv[6]);
	params->dongle_cooldown = atoi(argv[7]);
	params->burned_out = false;
	if (strcmp(argv[8], "edf") == 0)
		params->scheduler = EDF;
	else if (strcmp(argv[8], "fifo") == 0)
		params->scheduler = FIFO;
	else
		return (-1);
	return (0);
}
