GLOBAL prepareStack
GLOBAL haltCpu

global setFirstProcess

GLOBAL fireTimerInt

section .text


;uint64_t rsp, uint64_t rip, int argc, char* argv
;       ->rdi          ->rsi   ->rdx     ->rcx
prepareStack:
    push rbp
    mov rbp, rsp

    mov rsp, rdi ; new stack
    ;intento el align
    ;Comienzo a crear el nuevo stack
    push 0x0 ;SS
    mov rax, rsp
    add rax, 3
    push rax ;RSP final
    push 0x202 ;RFLAGS
    push 0x8 ;CS
    push rsi ; RIP
    ;push de los registros genelares

	push 0; rax
	push 0;rbx
	push 0;rcx
	push 0;rdx
	push 0;rbp
	push rdx;rdi (argc)
	push rcx;rsi (argv)
	push 0;r8
	push 0;r9
	push 0;r10
	push 0;r11
	push 0;r12
	push 0;r13
	push 0;r14
	push 0;r15
    mov rax, rsp 

    mov rsp, rbp
    pop rbp

    ret



haltCpu:
   hlt
   jmp haltCpu

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

setFirstProcess: 
    mov rsp, rdi 
    popState
    sti
	iretq


fireTimerInt:
	int 20h
	ret
