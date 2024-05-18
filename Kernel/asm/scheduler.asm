GLOBAL prepareStack

section .text

prepareStack:
    push rbp
    mov rbp, rsp

    mov rsp, rsi ; new stack
    ;intento el align
    ;Comienzo a crear el nuevo stack
    push 0x0 ;SS
    push rsp + 3 ;RSP final
    push 0x202 ;RFLAGS
    push 0x8 ;CS
    push rdi ; RIP
    ;falta ver tema registros generales
    mov eax, rsp 

    mov rsp, rbp
    pop rbp
    ret



haltCpu:
    hlt