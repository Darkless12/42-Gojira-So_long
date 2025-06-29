/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frames.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddiogo-f <ddiogo-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:30:30 by darkless12        #+#    #+#             */
/*   Updated: 2025/02/20 12:06:30 by ddiogo-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	get_pixel_color(t_tile *tile, int x, int y)
{
	int	offset;

	offset = (y * tile->line) + (x * (tile->bpp / 8));
	return (*(int *)(tile->addr + offset));
}

void	put_pixel(t_all *all, int x, int y, int color)
{
	int	offset;

	if (x >= 0 && x < all->plan.col * 64 && y >= 0 && y < all->plan.row * 64)
	{
		offset = (y * all->mlx.line) + (x * (all->mlx.bpp / 8));
		*(int *)(all->mlx.addr + offset) = color;
	}
}

void	blend_image(t_all *all, t_tile *tile, int len, int hei)
{
	int				x;
	int				y;
	int				color;
	unsigned char	alpha;

	y = 64 - tile->y;
	while (y < tile->y)
	{
		x = 64 - tile->x;
		while (x < tile->x)
		{
			color = get_pixel_color(tile, x + tile->x - 64, y + tile->y - 64);
			alpha = (color >> 24) & 0xFF;
			if (alpha != 0xFF)
				put_pixel(all, len + x, hei + y, color);
			x++;
		}
		y++;
	}
}

void	find_tile(t_all *all, int x, int y)
{
	int		sort;
	char	val;

	val = all->plan.map[y][x];
	sort = ((x * x) + (y * y * y));
	if (val == '1')
		blend_image(all, &all->wall[sort % 3], x * 64, y * 64);
	else if (val == '0' || val == '3')
		blend_image(all, &all->floor[(val - '0') + (sort % 3)], x * 64, y * 64);
	else if (val == 'C' || val == 'D')
		blend_image(all, &all->nuke[val - 'C'], x * 64, y * 64);
	else if ((val == 'E' || val == 'F') && all->plan.colect != all->game.nuke)
		blend_image(all, &all->floor[(val - 'E') + (sort % 3)], x * 64, y * 64);
	else if ((val == 'E' || val == 'F') && all->plan.colect == all->game.nuke)
		blend_image(all, &all->exit, x * 64, y * 64);
	else
		blend_image(all, &all->floor[sort % 3], x * 64, y * 64);
	if (all->game.x == x && all->game.y == y)
		blend_image(all, &all->gojira[all->game.dir], x * 64, y * 64);
}

int	render_frames(t_all *all)
{
	int	x;
	int	y;

	y = 0;
	all->mlx.img = mlx_new_image(all->mlx.ptr, all->plan.col * 64, \
		all->plan.row * 64);
	all->mlx.addr = mlx_get_data_addr(all->mlx.img, &all->mlx.bpp, \
		&all->mlx.line, &all->mlx.endian);
	while (y < all->plan.row)
	{
		x = 0;
		while (x < all->plan.col)
		{
			find_tile(all, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(all->mlx.ptr, all->mlx.win, all->mlx.img, 0, 0);
	mlx_destroy_image(all->mlx.ptr, all->mlx.img);
	return (0);
}
