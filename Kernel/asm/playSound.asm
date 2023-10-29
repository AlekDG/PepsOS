global silence
global speakerSet
global playback
speakerSet: ;En RDI esta el div
    push rbp
    mov rbp,rsp
    xor rax,rax
    mov rax,0xB6
    out 0x43,al
    mov al,[rdi]
    out 0x42,al
    xor rbx,rbx
    mov rbx,2
    div rbx
    and rax,0x000000FF
    div rbx
    and rax,0x000000FF
    div rbx
    and rax,0x000000FF
    out 0x42,al
    mov rsp,rbp
    pop rbp
    ret
silence:
    push rbp
    mov rbp,rsp
    xor rax,rax
    in al, 0x61
    and rax,0xFC
    out 0x61,al
    mov rsp,rbp
    pop rbp
    ret
playback:
    push rbp
    mov rbp,rsp
    xor rax,rax
    in al,0x61
    or rax,3
    out 0x61,al
    mov rsp,rbp
    pop rbp
    ret