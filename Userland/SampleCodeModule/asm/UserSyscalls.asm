GLOBAL call_regRead
GLOBAL call_clearScrean
GLOBAL call_drawRectangle
GLOBAL call_drawCircle
GLOBAL call_drawFace
GLOBAL call_getChar
GLOBAL call_wait
GLOBAL call_ticks
GLOBAL call_setSize
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
GLOBAL call_printInteger
GLOBAL call_drawLetterFormatted
GLOBAL call_drawStringFormatted
GLOBAL call_printIntFormatted
GLOBAL call_minutes
GLOBAL call_hours
GLOBAL call_day
GLOBAL call_month
GLOBAL call_year
GLOBAL call_excepDivZero
GLOBAL call_excepInvalidOp
GLOBAL call_free
GLOBAL call_malloc

%macro call_to_handler 1
    push rbp
    mov rbp, rsp
    mov r9, r8      ;arg 5
    mov r8, rcx     ;arg 4
    mov rcx, rdx    ;arg 3
    mov rdx, rsi    ;arg 2
    mov rsi, rdi    ;arg 1
    mov rdi, %1     ;syscall deseada
    int 80h         ;interrupcion 80
    mov rsp, rbp
	pop rbp
    ret
%endmacro

call_regRead:
    call_to_handler 1
call_clearScrean:
    call_to_handler 2
call_drawRectangle:
    call_to_handler 3
call_drawCircle:
    call_to_handler 4
call_drawFace:
    call_to_handler 5
call_getChar:
    call_to_handler 6
call_wait:
    call_to_handler 7
call_ticks:
    call_to_handler 8
call_setSize:
    call_to_handler 9
call_beep:
    call_to_handler 10
call_getWidth:
    call_to_handler 11
call_getHeight:
    call_to_handler 12
call_deleteLetterBuffered:
    call_to_handler 13
call_drawLetterFromChar:
    call_to_handler 14
call_getSize:
    call_to_handler 15
call_setXBuffer:
    call_to_handler 16
call_setYBuffer:
    call_to_handler 17
call_getXBuffer:
    call_to_handler 18
call_getYBuffer:
    call_to_handler 19
call_getBGColor:
    call_to_handler 20
call_getFGColor:
    call_to_handler 21
call_getFGColorPointer:
    call_to_handler 22
call_getXBufferPointer:
    call_to_handler 23
call_getYBufferPointer:
    call_to_handler 24
call_setFGColor:
    call_to_handler 25
call_setBGColor:
    call_to_handler 26
call_putpixelResizable:
    call_to_handler 27
call_seconds:
    call_to_handler 28
call_paintScreen:
    call_to_handler 29
call_printInteger:
    call_to_handler 30
call_drawLetterFormatted:
    call_to_handler 31
call_drawStringFormatted:
    call_to_handler 32
call_printIntFormatted:
    call_to_handler 33
call_minutes:
    call_to_handler 34
call_hours:
    call_to_handler 35
call_day:
    call_to_handler 36
call_month:
    call_to_handler 37
call_year:
    call_to_handler 38
call_malloc:
    call_to_handler 39
call_free:
    call_to_handler 40
call_excepDivZero:
    xor rdx,rdx
    div rdx
call_excepInvalidOp:
    mov cr6, rax