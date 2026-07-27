/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:59:33 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/19 16:47:22 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/tty.h>
#include <kernel/io.h>

void tty_move_cursor(unsigned short pos) {
	outb(0x3D4, 14);
	outb(0x3D5, ((pos >> 8) & 0x00FF));
	outb(0x3D4, 15);
	outb(0x3D5, pos & 0x00FF);
}

void	tty_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t i = y * VGA_WIDTH + x;

	tty[cur_tty].screen[i] = vga_entry(c, color);
	vga_buffer[i] = vga_entry(c, color);
	tty_move_cursor(i + 1);
}

static void	tty_scroll(void) {
	size_t	x;
	size_t	y;

	for (y = 1; y < VGA_HEIGHT; y++) {
		for (x = 0; x < VGA_WIDTH; x++)
			tty[cur_tty].screen[(y - 1) * VGA_WIDTH + x] =
				tty[cur_tty].screen[y * VGA_WIDTH + x];
	}
	for (x = 0; x < VGA_WIDTH; x++) {
		tty[cur_tty].screen[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
			vga_entry(' ', tty[cur_tty].color);
	}
	tty[cur_tty].row = VGA_HEIGHT - 1;
	tty[cur_tty].column = 0;
	tty_draw(&tty[cur_tty]);
}

static void	tty_printchar(char c) {
	tty_putentryat(c, tty[cur_tty].color, tty[cur_tty].column, tty[cur_tty].row);
	if (++tty[cur_tty].column == VGA_WIDTH) {
		tty[cur_tty].column = 0;
		if (++tty[cur_tty].row == VGA_HEIGHT)
			tty_scroll();
	}
}

static void	tty_newline() {
	size_t pos;

	if (tty[cur_tty].row == VGA_HEIGHT - 1)
		tty_scroll();
	else
		tty[cur_tty].row++;
	tty[cur_tty].column = 0;
	pos = tty[cur_tty].row * VGA_WIDTH + tty[cur_tty].column;
	tty_move_cursor(pos);
}

void tty_putchar(char c) {
	if (isprint(c)) {
		tty_printchar(c);
	} else if (c == '\n') {
		tty_newline();
	}
}

void	tty_backspace(void) {
	size_t	pos;

	if (tty[cur_tty].column == 0)
		return ;
	tty[cur_tty].column--;
	pos = tty[cur_tty].row * VGA_WIDTH + tty[cur_tty].column;
	tty[cur_tty].screen[pos] = vga_entry(' ', tty[cur_tty].color);
	vga_buffer[pos] = tty[cur_tty].screen[pos];
	tty_move_cursor(pos);
}
