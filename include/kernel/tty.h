/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 21:18:26 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/19 17:03:19 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <defines.h>
#include <libk.h>
#include <vga.h>

#define TTY_COUNT 4
#define TTY_INPUT_CAPACITY 256

typedef struct	s_tty {
	size_t		row;
	size_t		column;
	uint8_t		color;
	
	uint16_t	screen[VGA_WIDTH * VGA_HEIGHT];
	char		buffer[TTY_INPUT_CAPACITY];
	
}				t_tty;
extern t_tty	tty[TTY_COUNT];
extern volatile uint16_t	cur_tty;
extern uint16_t	*vga_buffer;

void tty_initialize(void);
void tty_clear(t_tty *t);
void tty_draw(t_tty *t);

void print_intro();
void tty_move_cursor(unsigned short pos);
void tty_switch_screen(int screen);

void tty_color(uint8_t color);
void tty_putchar(char c);
void tty_backspace(void);

#endif
