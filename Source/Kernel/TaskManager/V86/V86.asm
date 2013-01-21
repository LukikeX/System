[GLOBAL _v86_run]

_v86_run:
    mov rbx, 0x08
    push rbx
    pushfq
    pop rbx
    or rbx, 0x20200
    push rbx

    mov rbx, [rsi + 24]
    push rbx
    mov rbx, [rsi + 16]
    push rbx

    iretq