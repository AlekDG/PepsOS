GLOBAL call_read
GLOBAL call_write
GLOBAL call_time
GLOBAL call_regRead
GLOBAL call_clearScrean
GLOBAL call_drawRectangle
GLOBAL call_drawCenteredRectangle
GLOBAL call_drawCircle
GLOBAL call_drawFace
GLOBAL call_bufferPosition
GLOBAL call_getChar
GLOBAL call_wait
GLOBAL call_ticks
GLOBAL call_setTextSize
GLOBAL call_drawBufferedChar
GLOBAL call_beep

%macro handler_call 1
    push rbp
    mov rsp,rbp
    mov r9, r8  ;shift every arg to the next register
    mov r8, rcx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, %1 ;add the desired call number as the first arg
    int 80h
    mov rbp,rsp
    pop rbp
    ret
%endmacro

call_read:
    handler_call 1
call_write:
    handler_call 2
call_time:
    handler_call 3
call_regRead:
    handler_call 4
call_clearScrean:
    handler_call 5
call_drawRectangle:
    handler_call 6
call_drawCenteredRectangle:
    handler_call 7
call_drawCircle:
    handler_call 8
call_drawFace:
    handler_call 9
call_bufferPosition:
    handler_call 10
call_getChar:
    handler_call 11
call_wait:
    handler_call 12
call_ticks:
    handler_call 13
call_setTextSize:
    handler_call 14
call_drawBufferedChar:
    handler_call 15
call_beep:
    handler_call 16