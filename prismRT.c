/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prismRT.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:34:17 by aaycan            #+#    #+#             */
/*   Updated: 2026/07/30 23:07:27 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prismRT.h"

int	main(int argc, char **argv)
{
	if (!((argc == 2) && (!check_file(argv[1]))))
		error_message(1, "faulty file format");
	parse_scene(argv[1]);
	//print_the_scene(parse_scene(NULL));
	run_engine();
	free_scene();
	return (0);
}
