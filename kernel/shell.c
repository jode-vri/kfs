/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 17:22:25 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/23 19:29:10 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/tty.h>
#include <kernel/io.h>
#include <kernel/interrupts/keyboard.h>
#include <libk.h>
#include <kfs.h>

static void	read_line(void) {
	char		c;
	size_t		i;

	i = strlen(tty[cur_tty].buffer);
	while (42) {
		c = keyboard_state.c;
		keyboard_state.c = 0;
		if (c == 0)
			continue ;
		if (c == KEYBOARD_TTY_SWITCH) {
			i = strlen(tty[cur_tty].buffer);
			continue ;
		}
		if (c == '\n') {
			putchar(c);
			return ;
		}
		if (keyboard_state.ctrl) {
			if (c == 'l') {
				memset(tty[cur_tty].buffer, 0, TTY_INPUT_CAPACITY);
				tty_initialize();
				print_intro();
				return ;
			}
		}
		if (c == '\b') {
			if (i > 0) {
				tty[cur_tty].buffer[--i] = 0;
				tty_backspace();
			}
			continue ;
		}
		if (!isprint(c))
			continue ;
		if (i >= TTY_INPUT_CAPACITY - 1)
			continue ;
		putchar(c);
		tty[cur_tty].buffer[i++] = c;
		tty[cur_tty].buffer[i] = 0;
	}
}

static void run(void) {
	if (strcmp(tty[cur_tty].buffer, "shutdown") == 0) {
		outw(0x604, 0x2000);
	} else if (strcmp(tty[cur_tty].buffer, "reboot") == 0) {
		uint8_t good = 0x02;
		while (good & 0x02)
			good = inb(0x64);
		outb(0x64, 0xFE);
		asm("hlt");
	} else if (strcmp(tty[cur_tty].buffer, "stack") == 0) {
		int esp;
		int ebp;
		asm("mov %%esp, %0" : "=r"(esp) ::);
		asm("mov %%ebp, %0" : "=r"(ebp) ::);
		print_kernel(esp, ebp - esp);
	} else {
		printk("kfs: command not found: %s\n", tty[cur_tty].buffer);
	}
}

void		shell(void) {
	while (42) {
		printk("kfs > ");
		memset(tty[cur_tty].buffer, 0, TTY_INPUT_CAPACITY);
		read_line();
		if (strlen(tty[cur_tty].buffer) > 0){
			run();
		}
	}
}
