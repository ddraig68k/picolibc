/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2026 Keith Packard
 */

#include "m68k_dos_syscall.h"
#include <unistd.h>

int
isatty(int fd)
{
    return m68k_dos_syscall1(DOS_SYS_ISATTY, fd);
}
