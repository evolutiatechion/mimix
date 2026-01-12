# Mimix 3.1.2 Microkernel Operating System Documentation

## Overview
Mimix 3.1.2 is a modern microkernel operating system implemented using a hybrid approach combining:
- **NASM Assembly** for low-level kernel operations
- **GCC with ANSI C89/90** for higher-level system components
- **Standard C comments** (`/* ... */`) for documentation

## Architecture

### Kernel Structure
```assembly
/* ============================================
 * Mimix 3.1.2 Microkernel - Boot Section
 * File: kernel/boot.asm
 * Description: System initialization and bootstrap
 * ============================================ */

[BITS 32]
[ORG 0x100000]    /* Kernel load address */

section .text
global _start
_start:
    /* Setup protected mode */
    mov ax, 0x10   /* Kernel data segment */
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    /* Initialize kernel subsystems */
    call init_gdt      /* Global Descriptor Table */
    call init_idt      /* Interrupt Descriptor Table */
    call init_paging   /* Memory paging */
    
    /* Switch to C environment */
    extern kmain
    call kmain
    
    /* Should never return */
    cli
    hlt
```

### System Call Interface
```c
/* ============================================
 * Mimix System Call Interface
 * File: kernel/syscall.h
 * Description: System call definitions and handlers
 * Compiler: GCC with -std=c89 -pedantic
 * ============================================ */

#ifndef MIMIX_SYSCALL_H
#define MIMIX_SYSCALL_H

/* System call numbers */
#define SYSCALL_EXIT     0x00
#define SYSCALL_FORK     0x01
#define SYSCALL_READ     0x02
#define SYSCALL_WRITE    0x03
#define SYSCALL_OPEN     0x04
#define SYSCALL_CLOSE    0x05
#define SYSCALL_BRK      0x06

/* Maximum system calls */
#define MAX_SYSCALLS     256

/* System call prototype */
typedef int (*syscall_handler_t)(void);

/* System call table */
extern syscall_handler_t syscall_table[MAX_SYSCALLS];

/* System call registration */
int register_syscall(unsigned int num, syscall_handler_t handler);

#endif /* MIMIX_SYSCALL_H */
```

## Implementation Details

### 1. Interrupt Handling (Assembly)
```assembly
/* ============================================
 * Interrupt Service Routines
 * File: kernel/interrupt.asm
 * Description: Hardware interrupt handlers
 * ============================================ */

section .text

global isr_default
isr_default:
    /* Save all registers */
    pusha
    
    /* Call C handler */
    extern interrupt_handler
    call interrupt_handler
    
    /* Restore registers and return */
    popa
    add esp, 8  /* Cleanup error code and int number */
    iret

global syscall_handler
syscall_handler:
    /* System call dispatching */
    pusha
    
    /* Get system call number from eax */
    cmp eax, MAX_SYSCALLS
    jae .invalid_syscall
    
    /* Call handler from C table */
    extern syscall_table
    call [syscall_table + eax * 4]
    mov [esp + 28], eax  /* Store return value */
    
    popa
    iret
    
.invalid_syscall:
    mov eax, -1
    popa
    iret
```

### 2. Memory Management (C89)
```c
/* ============================================
 * Memory Management Unit
 * File: kernel/mm.c
 * Description: Physical and virtual memory management
 * Standards: ANSI C89/ISO C90
 * ============================================ */

#include "mm.h"

/* Memory region structure */
struct memory_region {
    void* start;
    void* end;
    int free;
    struct memory_region* next;
};

/* Initialize memory manager */
void mm_init(void* start, void* end)
{
    /* Create initial region */
    struct memory_region* region = 
        (struct memory_region*)start;
    
    region->start = start + sizeof(struct memory_region);
    region->end = end;
    region->free = 1;
    region->next = NULL;
    
    /* Initialize allocation tracking */
    g_mem_start = region;
    g_mem_end = end;
}

/* Allocate memory (first-fit algorithm) */
void* kmalloc(size_t size)
{
    struct memory_region* current = g_mem_start;
    
    while (current != NULL) {
        if (current->free) {
            size_t region_size = 
                (size_t)(current->end - current->start);
            
            if (region_size >= size) {
                /* Split region if large enough */
                if (region_size > size + sizeof(struct memory_region)) {
                    struct memory_region* new_region =
                        (struct memory_region*)(current->start + size);
                    
                    new_region->start = current->start + size + 
                                        sizeof(struct memory_region);
                    new_region->end = current->end;
                    new_region->free = 1;
                    new_region->next = current->next;
                    
                    current->end = current->start + size;
                    current->next = new_region;
                }
                
                current->free = 0;
                return current->start;
            }
        }
        current = current->next;
    }
    
    return NULL; /* Out of memory */
}
```

### 3. Process Management
```c
/* ============================================
 * Process Control Block
 * File: kernel/process.c
 * Description: Process scheduling and management
 * ============================================ */

#define MAX_PROCESSES 64

struct process {
    int pid;
    int state;
    void* stack_ptr;
    void* page_dir;
    struct process* next;
};

/* Process scheduler */
void schedule(void)
{
    struct process* current = get_current_process();
    struct process* next = current->next;
    
    /* Find next runnable process */
    while (next->state != PROCESS_RUNNABLE) {
        next = next->next;
        if (next == NULL) {
            next = process_list;
        }
    }
    
    /* Context switch */
    if (current != next) {
        switch_context(current, next);
    }
}
```

## Build System

### Makefile Configuration
```makefile
# Mimix 3.1.2 Build Configuration
CC = gcc
ASM = nasm
CFLAGS = -std=c89 -pedantic -Wall -Wextra -O2 -ffreestanding -nostdlib
ASFLAGS = -f elf32

# Source files
KERNEL_SOURCES = kernel/main.c kernel/mm.c kernel/process.c
ASM_SOURCES = kernel/boot.asm kernel/interrupt.asm

# Build targets
mimix.bin: kernel.elf
	objcopy -O binary kernel.elf mimix.bin

kernel.elf: $(ASM_SOURCES:.asm=.o) $(KERNEL_SOURCES:.c=.o)
	$(CC) -T linker.ld -o $@ $^ -lgcc

%.o: %.asm
	$(ASM) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
```

### Linker Script
```ld
/* Mimix 3.1.2 Kernel Linker Script */
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS {
    . = 0x100000;
    
    .text : {
        *(.text)
    }
    
    .rodata : {
        *(.rodata*)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        *(COMMON)
        *(.bss)
    }
    
    /DISCARD/ : {
        *(.note*)
        *(.comment*)
    }
}
```

## Key Features

### 1. Microkernel Design
- Minimal kernel running in privileged mode
- System services run in user space
- IPC via message passing
- Modular and extensible architecture

### 2. Standards Compliance
- **Assembly**: NASM syntax, Intel x86 architecture
- **C Language**: ANSI C89/ISO C90 compliant
- **Documentation**: Standard C comments throughout
- **Portability**: Hardware abstraction layer

### 3. Security Features
- Process isolation via memory protection
- Capability-based security model
- System call validation
- Inter-process communication safeguards

### 4. Performance Optimizations
- Preemptive multitasking
- Efficient context switching
- Optimized system call path
- Minimal interrupt latency

## Development Guidelines

### Code Style
```c
/* Function documentation must precede implementation */
int sys_write(int fd, const void* buf, size_t count)
{
    /* Input validation */
    if (buf == NULL) {
        return -1;  /* Invalid buffer */
    }
    
    /* Boundary checking */
    if (count > MAX_WRITE_SIZE) {
        return -2;  /* Size too large */
    }
    
    /* Implementation */
    return perform_write(fd, buf, count);
}

/* Structures should be clearly documented */
struct file_descriptor {
    int number;        /* File descriptor number */
    int flags;         /* Open flags */
    void* data;        /* File-specific data */
    struct vfs_node* node;  /* VFS node pointer */
};
```

### Assembly-C Interface
```assembly
/* Assembly functions called from C */
global enable_interrupts
enable_interrupts:
    sti     /* Enable interrupts */
    ret

global disable_interrupts
disable_interrupts:
    cli     /* Disable interrupts */
    ret

/* C functions called from assembly */
extern timer_handler  /* Defined in timer.c */
```

## System Requirements

### Hardware
- x86 32-bit processor (386+)
- 4MB RAM minimum
- VGA-compatible display
- Keyboard controller

### Toolchain
- NASM 2.10+
- GCC 4.8+ with C89 support
- GNU Make 3.81+
- QEMU for testing (recommended)

## Testing and Debugging

### Boot Testing
```bash
# Build the system
make clean
make

# Test in QEMU
qemu-system-i386 -kernel mimix.bin -m 16M

# Debug with GDB
qemu-system-i386 -kernel mimix.bin -s -S
gdb -ex "target remote localhost:1234"
```

### Debug Output
```c
/* Kernel debug printf implementation */
void kprintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    /* Simple serial output */
    while (*format) {
        if (*format == '%') {
            format++;
            /* Handle format specifiers */
        } else {
            serial_write(*format);
        }
        format++;
    }
    
    va_end(args);
}
```

This documentation provides a comprehensive overview of Mimix 3.1.2 microkernel OS implementation using modern NASM assembly and ANSI C89 with GCC. The system emphasizes clean separation between architecture-dependent assembly code and portable C components, all documented using standard C comments.
