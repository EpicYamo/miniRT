/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 16:34:17 by aaycan            #+#    #+#             */
/*   Updated: 2025/11/11 11:43:23 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	main(int argc, char **argv)
{
	if (!((argc == 2) && (!check_file(argv[1]))))
		error_message(1, "faulty file format");
	parse_scene(argv[1]);
	print_the_scene(parse_scene(NULL));
	run_engine();
	free_scene();
	return (0);
}
