/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgajowni <dgajowni@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:47:31 by dgajowni          #+#    #+#             */
/*   Updated: 2026/05/17 15:09:33 by dgajowni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	pthreads_join(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->number_of_coders)
		pthread_join(params->coders[i++].thread, NULL);
}

int	do_stuff(t_params *params)
{
	int	i;
	t_coder	*coders;

	i = -1;
	coders = params->coders;
	while (++i < params->number_of_coders)
	{
		if (coders[i].compiles_done < params->number_of_compiles_required)
		{
			if (pthread_create(
					&(coders[i].thread),
					NULL, coder_thread, &(coders[i])) != 0)
				return (-1);
		}
	}
	return(0);
}

int	main(int argc, char *argv[])
{
	t_params	params;

	gettimeofday(&params.time, NULL);
	if (parse_arguments(argc, argv, &params) < 0)
		return (print_error(argv[0]));
	if (init_mutexes(&params) < 0
		|| init_coders(&params) < 0)
	{
		destroy_mutexes(&params);
		free(params.coders);
		free(params.mutex);
	}
	do_stuff(&params);
	pthreads_join(&params);
	destroy_mutexes(&params);
	return (free(params.coders), free(params.mutex), 0);
}
