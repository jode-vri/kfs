/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pic.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:19:43 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/19 10:29:32 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/io.h>
#include <kernel/interrupts/pic.h>

void	pic_acknowledge(unsigned int interrupt) {
	if (interrupt < 0x20 || interrupt > 0x2F)
		return;
	if (interrupt >= 0x28)
		outb(0xA0, 0x20);
	outb(0x20, 0x20);
}

void	pic_remap(int offset1, int offset2) {
	// Initialize sequence for both master and slave PICs
    outb(0x20, 0x11);   // ICW1: starts the initialization sequence (in cascade mode)
    outb(0xA0, 0x11);

    // Set interrupt vector offsets for master and slave PICs
    outb(0x21, offset1); // ICW2: Master PIC vector offset
    outb(0xA1, offset2); // ICW2: Slave PIC vector offset

    // Tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(0x21, 0x04);   // ICW3
    // Tell Slave PIC its cascade identity (0000 0010)
    outb(0xA1, 0x02);   // ICW3

    // Enable (unmask) interrupts on the master and slave PICs
    outb(0x21, 0x01);   // ICW4
    outb(0xA1, 0x01);   // ICW4

    // Setup Interrupt Mask Register (IMR)
    outb(0x21, 0xFD);   // Enable only IRQ1 (keyboard) on the master PIC
    outb(0xA1, 0xFF);   // Disable all interrupts on the slave PIC

    asm("sti");         // Enable interrupts.
}
