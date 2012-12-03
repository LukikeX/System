[bits 64]
[extern interrupt_handler]

%macro ISR_NOERRCODE 1
    [global isr%1]

    isr%1:
        push qword 0
        push qword %1
        jmp interrupt_common_stub
%endmacro

%macro ISR_ERRCODE 1
    [global isr%1]

    isr%1:
        push qword %1
        jmp interrupt_common_stub
%endmacro

%macro IRQ 1
    [global irq%1]
    
    irq%1:
        push qword 0
        push qword 32+%1
        jmp interrupt_common_stub
%endmacro

%macro SYSCALL 1
    [global int%1]
    
    int%1:
        push qword 0
        push qword %1
        jmp interrupt_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

SYSCALL 66
SYSCALL 77
SYSCALL 88


interrupt_common_stub:
    push rdi
    push rsi
    push rbp
    push rsp
    push rdx
    push rcx
    push rbx
    push rax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rdi, rsp
    call interrupt_handler

    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rsp
    pop rbp
    pop rsi
    pop rdi

    add rsp, 16
    iretq