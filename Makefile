# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jode-vri <jode-vri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/02 20:57:26 by jode-vri          #+#    #+#              #
#    Updated: 2023/12/19 17:25:05 by jode-vri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	kfs
ISO			=	$(NAME).iso

CC			?=	gcc
CPPFLAGS	+=	-I$(HEADERS)
CFLAGS		+=	-m32 -std=gnu11 -ffreestanding -nostdlib -nodefaultlibs
CFLAGS		+=	-fno-builtin -fno-exceptions -fno-stack-protector
CFLAGS		+=	-fno-pie -fno-pic -Wall -Wextra -Werror -g3 -MMD -MP

NASM		?=	nasm
NASM_FLAGS	=	-f elf32

LD			?=	ld
LDFLAGS		=	-m elf_i386 --build-id=none
LINKER		=	kernel/arch/i386/linker.ld
GRUB_FILE	?=	grub-file
QEMU		?=	qemu-system-i386
READELF		?=	readelf

HEADERS		=	include
ISO_DIR		=	iso

BOOT		=	kernel/arch/i386/boot/boot.asm
BOOT_OBJ	=	kernel/arch/i386/boot/boot.o

SRCS		=	$(shell find kernel -type f -name '*.c')
SRCS		+=	$(shell find libk -type f -name '*.c')
SRCS_ASM	=	$(shell find kernel/arch/i386 -type f -name '*.s')

OBJS		=	$(patsubst %.c,%.o,$(SRCS))
OBJS		+=	$(patsubst %.s,%.o,$(SRCS_ASM))
DEPS		=	$(patsubst %.c,%.d,$(SRCS))

%.o: %.c
	@echo "Creating $@ ..."
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.s
	@echo "Creating $@ ..."
	@$(NASM) $(NASM_FLAGS) $< -o $@

all: boot $(OBJS) linker iso

build: boot $(OBJS) linker

boot:	$(BOOT)
	@echo "Creating $(BOOT_OBJ)..."
	@$(NASM) -f elf32 $(BOOT) -o $(BOOT_OBJ)

linker: $(LINKER) $(BOOT_OBJ) $(OBJS)
	@echo "Linking ..."
	@$(LD) $(LDFLAGS) -T $(LINKER) -o $(NAME) $(BOOT_OBJ) $(OBJS)
	@echo "$(NAME) binary is ready !"

iso:
	@echo "Creating the $(ISO_DIR) directory"
	@mkdir -pv $(ISO_DIR)/boot/grub
	@cp $(NAME) $(ISO_DIR)/boot
	@cp grub.cfg $(ISO_DIR)/boot/grub
	@echo "Creating the $(ISO) file"
	@grub-mkrescue -o $(ISO) --compress=xz $(ISO_DIR)

run:
	@$(QEMU) -cdrom $(ISO)

check: all
	@$(GRUB_FILE) --is-x86-multiboot $(NAME)
	@! $(READELF) -lW $(NAME) | grep -Eq 'LOAD.*RWE'
	@echo "Multiboot header and ELF segment permissions are valid."

smoke-test: check
	@./scripts/smoke-test.sh

clean:
	@echo "Cleaning Objs ..."
	@rm -rf $(OBJS) $(DEPS) $(BOOT_OBJ) $(ISO_DIR)

fclean: clean
	@echo "Cleaning kfs ..."
	@rm -rf $(NAME) $(ISO)

re: fclean all

-include $(DEPS)

.PHONY: all build boot linker iso run check smoke-test clean fclean re
