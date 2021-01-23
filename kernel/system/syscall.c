#include <system/syscall.h>
#include <int/regs.h>
#include <int/int.h>
#include <kernel/irq.h>
#include <kernel/idt.h>

#include <stdio.h>

static void syscall_handler(regs_t*);
static void hello(void);

static void *syscalls[MAX_SYSCALLS] = {
    &hello
};

void syscall_bootstrap(void)
{
    // register page fault handler
    irq_handler_t handler;
    handler = &syscall_handler;
    isr_install_handler(INT_SYSCALL, handler);
}

static void syscall_handler(regs_t *r)
{
    // Requested syscall number is in EAX
    if (r->eax >= MAX_SYSCALLS)
        return;

    void *code = syscalls[r->eax];

    uint32_t ret;

    asm volatile (" \
        push %1; \
        push %2; \
        push %3; \
        push %4; \
        push %5; \
        call *%6; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
        pop %%ebx; \
    " : "=a" (ret) : "r" (r->edi), "r" (r->esi), "r" (r->edx), "r" (r->ecx), "r" (r->ebx), "r" (code));

    r->eax = ret;
}

static void hello(void)
{
    printf("hello world from userspace!\n");
}
