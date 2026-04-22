/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2026 Keith Packard
 */

#include <picolibc.h>

	.section .text.m68k_dos_syscall
	.global m68k_dos_syscall
	.type m68k_dos_syscall, @function
	.balign 4
m68k_dos_syscall:
	.cfi_startproc
	movel %sp@(4),%d0
	movel %sp@(8),%d1
	trap #15
	rts
	.cfi_endproc
	.size m68k_dos_syscall, .- m68k_dos_syscall

#if defined(__linux__) && defined(__ELF__)
	.section .note.GNU-stack,"",%progbits
#endif
