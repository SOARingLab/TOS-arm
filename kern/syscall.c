#include <syscall.h>
// #include <unistd.h>

#include "console.h"
#include "trap.h"
#include "sd.h"
#include "debug.h"

int
execve(const char *path, char *const argv[], char *const envp[])
{
    cprintf("- execve: path=0x%p, argv=0x%p, envp=0x%p\n", path, argv, envp);
    // TODO: check path

    // FIXME:
    static struct spinlock lock;
    static int first = 1;
    int do_test = 0;
    acquire(&lock);
    if (first) {
        do_test = 1;
        first = 0;
    }
    release(&lock);
    if (do_test) sd_test();
    struct buf b;
    b.flags = 0;
    b.blockno = 0;
    sdrw(&b);
    debug_mem(b.data, sizeof(b.data));

    return 0xad;
}

int
syscall1(struct trapframe *tf)
{
    int sysno = tf->x[8];
    switch (sysno) {
    case SYS_execve:
        return execve(tf->x[0], tf->x[1], tf->x[2]);
    case SYS_sched_yield:
        yield();
        return 0;
    default:
        cprintf("Unexpected syscall #%d\n", sysno);
    }
}
