/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 14:27:04 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/19 16:28:26 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <defines.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_TTY_SWITCH (-42)

typedef struct keyboard_state {
	uint8_t	shift;
	uint8_t	ctrl;
	uint8_t	alt;
	uint8_t	caps_lock;
	char	c;
} t_keyboard_state;
extern volatile t_keyboard_state	keyboard_state;


uint8_t keyboard_read(void);
uint8_t keyboard_get_char(uint8_t code, uint8_t shift);
void	handle_keypress(uint8_t code);

#endif
