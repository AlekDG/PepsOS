GLOBAL clock
GLOBAL cpuVendor
GLOBAL getKey
GLOBAL outb
GLOBAL inb
GLOBAL userBuild
EXTERN getStackBase
GLOBAL enter_region
GLOBAL leave_region
EXTERN sem_miss
section .text
	
clock:
	push rbp
	mov rbp, rsp

	mov al, dil
	out 70h, al
	xor rax, rax
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret


cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret


getKey:
        push rbp
        mov rbp, rsp
        xor rax,rax
        in al, 0x60
        mov rsp, rbp
        pop rbp
        ret
outb:
	push rbp
	mov rbp,rsp
	mov rax,rsi
	mov rdx,rdi
	out dx,al
	mov rsp,rbp
	pop rbp
	ret

inb:
	push rbp
	mov rbp,rsp
	mov rdx,rdi
	in al,dx
	mov rsp,rbp
	pop rbp
	ret

userBuild:
	add rsp, 32
	mov rax, 0x400000
    mov [rsp], rax          
    mov rax, 0x8
    mov [rsp + 8], rax      
    mov rax, 0x202
    mov [rsp + 8*2], rax    
    call getStackBase       
    mov [rsp + 8*3], rax    
    mov rax, 0x0
    mov [rsp + 8*4], rax    
	iretq

enter_region:
	mov rax, lockd
	xchg rax,[rdi]
	cmp rax,lockd
	jne .hit
	push rdi
	mov rdi,rsi
	push rsi
	call sem_miss
	pop rsi
	pop rdi
	jmp .miss

.hit:
	sti
	ret

.miss:
	int 20h
	jmp enter_region

leave_region:
	push rbp
	mov rbp,rsp
	mov rax,unlockd
	mov qword [rdi],rax
	mov rsp,rbp
	pop rbp
	ret

section .data
	lockd equ 0x01
	unlockd equ 0x00