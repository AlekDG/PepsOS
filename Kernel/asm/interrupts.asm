
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq60Handler

GLOBAL _exception00Handler
GLOBAL _exception06Handler
EXTERN saveRegsBuffer
EXTERN regCheckSave
EXTERN irqDispatcher
EXTERN sysIntDispatcher
EXTERN exceptionDispatcher
EXTERN getStackBase

EXTERN scheduler
EXTERN priorityScheduler

EXTERN timer_handler

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



%macro exceptionHandler 1
	pushState
	mov rsi,rsp
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher
	popState
	call getStackBase
	mov qword [rsp+8*3], rax
	mov qword [rsp], 0x400000
	iretq
%endmacro

fixStackBase:


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	pushState

	mov rdi, rsp ; pasaje de parametro
	mov rsi, rbp ;
	call priorityScheduler
	mov rsp, rax

	call timer_handler

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

;Keyboard
_irq01Handler:
	pushState
	mov rdi,rsp
	call regCheckSave
	popState
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


_irq60Handler:
	push rbx
	push r12
	push r13
	push r14
	push r15
	push rbp

	mov rbp, rsp
	push r9
	mov r9, r8
	mov r8, rcx
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 60h
	call irqDispatcher


	pop r9
    mov rsp, rbp
	pop rbp
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	iretq
;Zero Division Exception
_exception00Handler:
	exceptionHandler 0
_exception06Handler:
	exceptionHandler 6
haltcpu:
	cli
	hlt
	ret

SECTION .bss
	reserve resq 4