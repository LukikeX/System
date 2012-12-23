[bits 64]
[extern Loader]

jmp Loader

[global _program_test]
_program_test:
   ; int 64
    ;ret
    jmp 0