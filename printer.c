/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 15:34:51 by dgajowni          #+#    #+#             */
/*   Updated: 2026/02/10 18:41:44 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	printfm(t_params params, const char *str, long timestamp, int id)
{
	int	result;

	pthread_mutex_lock(&(params.print_mutex));
	result = printf(str, timestamp, id);
	pthread_mutex_unlock(&(params.print_mutex));
	return (result);
}

int	print_arguments(t_params *params)
{
	printf("Number of coders: %d\n", params->number_of_coders);
	printf("Time to burnout: %d ms\n", params->time_to_burnout);
	printf("Time to compile: %d ms\n", params->time_to_compile);
	printf("Time to debug: %d ms\n", params->time_to_debug);
	printf("Time to refactor: %d ms\n", params->time_to_refactor);
	printf("Number of compiles required: %d\n",
		params->number_of_compiles_required);
	if (params->scheduler == EDF)
		printf("Scheduler: %s\n", "EDF");
	else
		printf("Scheduler: %s\n", "FIFO");
	return (0);
}

int	print_error(char *program_name)
{
	fprintf(stderr, "Usage: %s number_of_coders \
time_to_burnout time_to_compile time_to_debug \
time_to_refactor number_of_compiles_required \
scheduler\n", program_name);
	return (-1);
}
