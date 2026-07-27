/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_kernel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 19:17:30 by jode-vri          #+#    #+#             */
/*   Updated: 2023/12/23 19:40:31 by jode-vri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libk.h>

static void print_hexa(uint32_t addr, char *buffer, int size) {
    int     len;
    char    base[16] = "0123456789abcdef";

    len = size - 1;
    memset(buffer, '0', size);
    buffer[size - 1] = 0;
    while (addr != 0) {
        buffer[--len] = base[addr % 16];
        addr /= 16;
    }
}

void    print_kernel(uint32_t addr, int limit) {
    char        *c = (char *)addr;
    char        buffer[9];
    int         i;
    uint32_t    last;
    
    if (limit <= 0)
        return ;
    for (i = 0; i < limit; i++) {
        if (i % 16 == 0) {
            if(i != 0) {
                last = addr - 16;
                while (last < addr) {
                    if (*(uint8_t *)last < 32 || *(uint8_t *)last >= 127)
                        printk(".");
                    else
                        printk("%c", *(char *)last);
                    last++;
                }
                printk("\n");
            }
            printk("%p: ", addr);
        }
        print_hexa((uint8_t)c[i], buffer, 3);
        printk("%s ", buffer);
        addr++;
    }
    for (i = 0; i < ((limit % 16) * 3); i++) {
        printk(" ");
    }
    if (limit % 16 == 0)
        last = addr - 16;
    else
        last = addr - (limit % 16);
    while (last < addr) {
        if (*(uint8_t *)last < 32 || *(uint8_t *)last >= 127)
            printk(".");
        else
            printk("%c", *(char *)last);
        last++;
    }
    printk("\n");
}
