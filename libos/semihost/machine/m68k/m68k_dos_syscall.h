/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2026 Keith Packard
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _M68K_DOS_SYSCALL_H_
#define _M68K_DOS_SYSCALL_H_

#include <stdio.h>
#include <errno.h>
#include <endian.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * DOS trap operation numbers. Keep defaults aligned with the existing
 * semihost protocol so early bring-up can reuse existing host tooling.
 */
#define DOS_SYS_EXIT         0
#define DOS_SYS_INIT_SIM     1
#define DOS_SYS_OPEN         2
#define DOS_SYS_CLOSE        3
#define DOS_SYS_READ         4
#define DOS_SYS_WRITE        5
#define DOS_SYS_LSEEK        6
#define DOS_SYS_RENAME       7
#define DOS_SYS_UNLINK       8
#define DOS_SYS_STAT         9
#define DOS_SYS_FSTAT        10
#define DOS_SYS_GETTIMEOFDAY 11
#define DOS_SYS_ISATTY       12
#define DOS_SYS_SYSTEM       13

/*
 * Open flag mapping is retained so existing newlib-style glue can be
 * adapted incrementally.
 */
#define GDB_O_RDONLY        0
#define GDB_O_WRONLY        1
#define GDB_O_RDWR          2
#define GDB_O_APPEND        8
#define GDB_O_CREAT         0x200
#define GDB_O_TRUNC         0x400
#define GDB_O_EXCL          0x800

typedef uint32_t my_mode_t;
typedef uint32_t my_time_t;

struct m68k_stat {
    uint32_t  my_dev;
    uint32_t  my_ino;
    my_mode_t my_mode;
    uint32_t  my_nlink;
    uint32_t  my_uid;
    uint32_t  my_gid;
    uint32_t  my_rdev;
    uint64_t  my_size;
    uint64_t  my_blksize;
    uint64_t  my_blocks;
    my_time_t my_atime;
    my_time_t my_mtime;
    my_time_t my_ctime;
} __packed;

static inline void
copy_stat(struct stat * restrict statbuf, struct m68k_stat *m68k_stat)
{
    statbuf->st_dev = be32toh(m68k_stat->my_dev);
    statbuf->st_ino = be32toh(m68k_stat->my_ino);
    statbuf->st_mode = be32toh(m68k_stat->my_mode);
    statbuf->st_nlink = be32toh(m68k_stat->my_nlink);
    statbuf->st_uid = be32toh(m68k_stat->my_uid);
    statbuf->st_gid = be32toh(m68k_stat->my_gid);
    statbuf->st_rdev = be32toh(m68k_stat->my_rdev);
    statbuf->st_size = be32toh(m68k_stat->my_size);
    statbuf->st_blksize = be32toh(m68k_stat->my_blksize);
    statbuf->st_blocks = be32toh(m68k_stat->my_blocks);
    statbuf->st_atime = be32toh(m68k_stat->my_atime);
    statbuf->st_mtime = be32toh(m68k_stat->my_mtime);
    statbuf->st_ctime = be32toh(m68k_stat->my_ctime);
}

struct m68k_dos_syscall_args {
    uintptr_t args[4];
};

intptr_t m68k_dos_syscall(int func, struct m68k_dos_syscall_args *args);

static inline intptr_t
m68k_dos_syscall1_immediate(int func, uintptr_t arg0)
{
    return m68k_dos_syscall(func, (struct m68k_dos_syscall_args *)arg0);
};

static inline intptr_t
m68k_dos_syscall1(int func, uintptr_t arg0)
{
    struct m68k_dos_syscall_args args;
    intptr_t                     ret;

    args.args[0] = arg0;
    m68k_dos_syscall(func, &args);
    ret = args.args[0];
    if (ret < 0)
        errno = args.args[1];
    return ret;
};

static inline intptr_t
m68k_dos_syscall2(int func, uintptr_t arg0, uintptr_t arg1)
{
    struct m68k_dos_syscall_args args;
    intptr_t                     ret;

    args.args[0] = arg0;
    args.args[1] = arg1;
    m68k_dos_syscall(func, &args);
    ret = args.args[0];
    if (ret < 0)
        errno = args.args[1];
    return ret;
};

static inline intptr_t
m68k_dos_syscall3(int func, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
    struct m68k_dos_syscall_args args;
    intptr_t                     ret;

    args.args[0] = arg0;
    args.args[1] = arg1;
    args.args[2] = arg2;
    m68k_dos_syscall(func, &args);
    ret = args.args[0];
    if (ret < 0)
        errno = args.args[1];
    return ret;
};

static inline uint64_t
m68k_dos_syscall4_64(int func, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
{
    struct m68k_dos_syscall_args args;
    uint64_t                     ret;

    args.args[0] = arg0;
    args.args[1] = arg1;
    args.args[2] = arg2;
    args.args[3] = arg3;
    m68k_dos_syscall(func, &args);
    ret = ((uint64_t)args.args[0] << 32) | args.args[1];
    if ((int64_t)ret < 0)
        errno = args.args[2];
    return ret;
};

static inline intptr_t
m68k_dos_syscall4(int func, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
{
    struct m68k_dos_syscall_args args;
    intptr_t                     ret;

    args.args[0] = arg0;
    args.args[1] = arg1;
    args.args[2] = arg2;
    args.args[3] = arg3;
    m68k_dos_syscall(func, &args);
    ret = args.args[0];
    if (ret < 0)
        errno = args.args[1];
    return ret;
};

#endif /* _M68K_DOS_SYSCALL_H_ */
