GLOBAL call_read
GLOBAL call_write
GLOBAL call_time
GLOBAL call_regRead
GLOBAL call_clearScrean
GLOBAL call_drawRectangle
GLOBAL call_drawCenteredRectangle
GLOBAL call_drawCircle
GLOBAL call_drawFace
GLOBAL call_getChar
GLOBAL call_wait
GLOBAL call_ticks
GLOBAL call_setSize
GLOBAL call_drawBufferedChar
GLOBAL call_beep
GLOBAL call_getWidth
GLOBAL call_getHeight
GLOBAL call_deleteLetterBuffered
GLOBAL call_drawLetterFromChar
GLOBAL call_getSize
GLOBAL call_setXBuffer
GLOBAL call_setYBuffer
GLOBAL call_getXBuffer
GLOBAL call_getYBuffer
GLOBAL call_getBGColor
GLOBAL call_getFGColor
GLOBAL call_getFGColorPointer
GLOBAL call_getXBufferPointer
GLOBAL call_getYBufferPointer
GLOBAL call_setFGColor
GLOBAL call_setBGColor
GLOBAL call_putpixelResizable
GLOBAL call_seconds
GLOBAL call_paintScreen

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
call_getChar:
    handler_call 10
call_wait:
    handler_call 11
call_ticks:
    handler_call 12
call_setSize:
    handler_call 13
call_drawBufferedChar:
    handler_call 14
call_beep:
    handler_call 15
call_getWidth:
    handler_call 16
call_getHeight:
    handler_call 17
call_deleteLetterBuffered:
    handler_call 18
call_drawLetterFromChar:
    handler_call 19
call_getSize:
    handler_call 20
call_setXBuffer:
    handler_call 21
call_setYBuffer:
    handler_call 22
call_getXBuffer:
    handler_call 23
call_getYBuffer:
    handler_call 24
call_getBGColor:
    handler_call 25
call_getFGColor:
    handler_call 26
call_getFGColorPointer:
    handler_call 27
call_getXBufferPointer:
    handler_call 28
call_getYBufferPointer:
    handler_call 29
call_setFGColor:
    handler_call 30
call_setBGColor:
    handler_call 31
call_putpixelResizable:
    handler_call 32
call_seconds:
    handler_call 33
call_paintScreen:
    handler_call 34