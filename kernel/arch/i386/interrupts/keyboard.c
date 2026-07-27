/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 14:29:04 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/20 19:06:56 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/interrupts/keyboard.h>
#include <kernel/tty.h>
#include <kernel/io.h>
#include <libk.h>

volatile t_keyboard_state	keyboard_state;

char qwerty_table[128] = {
	0x0, 0x0, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', '\b', '\t',
	'q','w','e','r','t','y','u','i',
	'o','p','[',']','\n', -2, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', -1, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', -1, '*',
	0,' ', -3, -4, -5, -6, -7, 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0, '-', 0x0, 0, 0x0, '+', 0x0,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x0, 0x0
};

char qwerty_shift_table[128] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&',
	'*', '(', ')', '_', '+', '\b', 0, 'Q',
	'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O',
	'P', '{', '}', '\n', 0, 'A', 'S', 'D',
	'F', 'G', 'H', 'J', 'K', 'L', ':', '\"',
	'~', 0, '|', 'Z', 'X', 'C', 'V', 'B',
	'N', 'M', '<', '>', '?', 0, '*', 0,
	' ', 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, '7', '8',
	'9', '-', '4', '5', '6', '+', '1', '2',
	'3', '0', '.', '6', 0, 0, 0, 0, 0
};

uint8_t	keyboard_read(void) {
	uint16_t	code;
	char		*table;

	code = inb(KEYBOARD_DATA_PORT);
	table = qwerty_table;
	if (code & 0x80) {
		switch (table[code ^ 0x80]) {
			case -1:
				keyboard_state.shift = 0;
				break;
			case -2:
				keyboard_state.ctrl = 0;
				break;
		}
	} else {
		outb(KEYBOARD_DATA_PORT, 0);
		if (code > 128)
			return (0);
		switch (table[code]) {
			case -1:
				keyboard_state.shift = 1;
				break;
			case -2:
				keyboard_state.ctrl = 1;
				break;
			case -3:
				keyboard_state.caps_lock = !keyboard_state.caps_lock;
				break;
		}
	}
	return (code);
}

void	handle_keypress(uint8_t code) {
	char	c;

	if (code == 0 || code >= 128)
		return ;
	if (code >= 59 && code <= 62){
		keyboard_state.c = KEYBOARD_TTY_SWITCH;
		tty_switch_screen(code);
		return ;
	}
	if (keyboard_state.shift || keyboard_state.caps_lock)
		c = qwerty_shift_table[code];
	else
		c = qwerty_table[code];
	keyboard_state.c = c;
}
