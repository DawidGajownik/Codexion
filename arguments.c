/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:47:42 by dgajowni          #+#    #+#             */
/*   Updated: 2026/02/08 15:28:35 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_arguments(int argc, char *argv[], t_params *params)
{
	if (argc < 8 || argc > 8)
		return (-1);
	params->number_of_coders = atoi(argv[1]);
	params->time_to_burnout = atoi(argv[2]);
	params->time_to_compile = atoi(argv[3]);
	params->time_to_debug = atoi(argv[4]);
	params->time_to_refactor = atoi(argv[5]);
	params->number_of_compiles_required = atoi(argv[6]);
	if (strcmp(argv[7], "edf") == 0)
		params->scheduler = EDF;
	else if (strcmp(argv[7], "fifo") == 0)
		params->scheduler = FIFO;
	else
		return (-1);
	return (0);
}
