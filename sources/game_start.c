/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:33:55 by darkless12        #+#    #+#             */
/*   Updated: 2025/02/20 12:13:28 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	end_game(t_all *all)
{
	render_frames(all);
	write(1, "YOU WIN!!\n", 11);
	close_it(all);
}

void	move_it(t_all *all, int x, int y, int d)
{
	if (all->plan.map[all->game.y + y][all->game.x + x] != '1')
	{
		all->game.y += y;
		all->game.x += x;
		all->game.dir = d;
		all->game.moves += 1;
		if (all->plan.map[all->game.y][all->game.x] == '0')
			all->plan.map[all->game.y][all->game.x] = '3';
		if (all->plan.map[all->game.y][all->game.x] == 'C')
		{
			all->plan.map[all->game.y][all->game.x] = 'D';
			all->game.nuke += 1;
		}
		if (all->plan.map[all->game.y][all->game.x] == 'E' || \
			all->plan.map[all->game.y][all->game.x] == 'F')
		{
			if (all->plan.colect == all->game.nuke)
				end_game(all);
			else
				all->plan.map[all->game.y][all->game.x] = 'F';
		}
		render_frames(all);
		ft_putnbr_fd(all->game.moves, 1);
		write(1, " moves\n", 7);
	}
}

int	handle_keys(int key, t_all *all)
{
	if (key == 65307)
		close_it(all);
	else if (key == 119)
		move_it(all, 0, -1, 2);
	else if (key == 115)
		move_it(all, 0, 1, 3);
	else if (key == 97)
		move_it(all, -1, 0, 0);
	else if (key == 100)
		move_it(all, 1, 0, 1);
	return (0);
}

int	game_start(t_all *all)
{
	all->mlx.ptr = mlx_init();
	all->mlx.win = mlx_new_window(all->mlx.ptr, all->plan.col * 64, \
		all->plan.row * 64, "Gojira So Long");
	mlx_do_key_autorepeatoff(all->mlx.ptr);
	load_textures(all);
	render_frames(all);
	mlx_hook(all->mlx.win, 2, 1L << 0, handle_keys, all);
	mlx_hook(all->mlx.win, 17, 0L, close_it, all);
	mlx_loop(all->mlx.ptr);
	return (0);
}
