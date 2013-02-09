[GLOBAL _v86_run]

_v86_run:
    pop rax
    pop rax

    xor rbx, rbx
    mov bx, [rax + 20]
    push rbx
    mov bx, [rax + 18]
    push rbx
    mov bx, [rax + 14]
    push rbx
    mov bx, [rax + 16]
    push rbx
    mov bx, [rax + 22]
    push rbx
    mov bx, [rax + 26]
    push rbx

    pushfq
    pop rbx
   or rbx, 0x20200
    push rbx

    xor rbx, rbx
    mov bx, [rax + 12] ; CS
    push rbx
    mov bx, [rax + 24] ; IP
    push rbx

    mov bx, [rax + 2]
    mov cx, [rax + 4]
    mov dx, [rax + 6]
    mov di, [rax + 8]
    mov si, [rax + 10]
    mov ax, [rax]

    iretq