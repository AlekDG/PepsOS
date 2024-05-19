GLOBAL prepareStack
GLOBAL haltCpu

section .text

prepareStack:
    push rbp
    mov rbp, rsp

    mov rsp, rsi ; new stack
    ;intento el align
    ;Comienzo a crear el nuevo stack
    push 0x0 ;SS
    mov rax, rsp
    add rax, 3
    push rax ;RSP final
    push 0x202 ;RFLAGS
    push 0x8 ;CS
    push rdi ; RIP
    ;falta ver tema registros generales
    mov rax, rsp 

    mov rsp, rbp
    pop rbp
    ret



haltCpu:
    hlt