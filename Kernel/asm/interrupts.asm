
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

GLOBAL _exception0Handler

EXTERN irqDispatcher
EXTERN sysIntDispatcher
EXTERN exceptionDispatcher

SECTION .text

%macro regToStack 0
	mov [regs.drbp], rbp
	mov rbp, [rsp]
	mov [regs.dr15], rbp
	mov rbp, [rsp+8]
	mov [regs.dr14], rbp
	mov rbp, [rsp+16]
	mov [regs.dr13], rbp
	mov rbp, [rsp+24]
	mov [regs.dr12], rbp
	mov rbp, [rsp+32]
	mov [regs.dr11], rbp
	mov rbp, [rsp+40]
	mov [regs.dr10], rbp
	mov rbp, [rsp+48]
	mov [regs.dr9], rbp
	mov rbp, [rsp+56]
	mov [regs.dr8], rbp
	mov rbp, [rsp+64]
	mov [regs.drsi], rbp
	mov rbp, [rsp+72]
	mov [regs.drdi], rbp
	mov rbp, [rsp+88]
	mov [regs.drdx], rbp
	mov rbp, [rsp+96]
	mov [regs.drcx], rbp
	mov rbp, [rsp+104]
	mov [regs.drbx], rbp
	mov rbp, [rsp+112]
	mov [regs.drax], rbp
	mov rbp, [rsp+120]
	mov [regs.drip], rbp
	mov rbp, [rsp+128]
	mov [regs.dcs], rbp
	mov rbp, [rsp+136]
	mov [regs.drfl], rbp
	mov rbp, [rsp+144]
	mov [regs.drsp], rbp
	mov rbp, [rsp+152]
	mov [regs.dss], rbp
	mov rbp, [regs.drbp]
%endmacro

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
	regToStack
	mov qword rdi,0x0000FF
	mov rsi,regs
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher
	popState
	iretq
%endmacro


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
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
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

saveRegs:
	pushState
	regToStack
	popState
	ret

SECTION .bss
	reserve resq 4

	GLOBAL regs
	regs:
	.drax resq 1
	.drbx resq 1
	.drcx resq 1
	.drdx resq 1
	.drsi resq 1
	.drdi resq 1
	.drsp resq 1
	.drbp resq 1
	.dr8  resq 1
	.dr9  resq 1
	.dr10 resq 1
	.dr11 resq 1
	.dr12 resq 1
	.dr13 resq 1
	.dr14 resq 1
	.dr15 resq 1
	.dss  resq 1
	.dcs  resq 1
	.drfl resq 1
	.drip resq 1