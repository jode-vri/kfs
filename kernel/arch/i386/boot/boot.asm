global start
extern kmain

section .multiboot
	align 4
	dd 0x1BADB002
	dd 0x0
	dd -(0x1BADB002 + 0x0)

section .text
	start:
		cli
		mov esp, kernel_stack_top
		xor ebp, ebp
		call kmain

	.hang:
		cli
		hlt
		jmp .hang

section .bss
	align 16
	kernel_stack_bottom:
	resb 16384
	kernel_stack_top:
