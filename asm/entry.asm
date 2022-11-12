[bits 32]

; Declare constants for the multiboot header.
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field

MAGIC_HEADER       equ  0x1BADB002
CHECKSUM    equ -(MAGIC_HEADER + FLAGS)

BOOTLOADER_MAGIC  equ  0x2BADB002


; set multiboot section
section .multiboot
    align 4
    dd MAGIC_HEADER
    dd FLAGS
    dd CHECKSUM

section .data
    align 4096

; initial stack
section .initial_stack, nobits
    align 4

stack_bottom:
    ; 1 MB of uninitialized data for stack
    resb 104856
stack_top:

; kernel entry, main text section
section .text
    global _start


; define _start, aligned by linker.ld script
_start:
    mov esp, stack_top
    extern kmain
    call kmain
loop:
    jmp loop