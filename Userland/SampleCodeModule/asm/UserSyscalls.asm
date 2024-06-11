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
GLOBAL call_printHex
GLOBAL call_print_long_long_int
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
GLOBAL call_mem_state

GLOBAL call_createProcess

GLOBAL call_getPid
global call_createBackgroundProcess
global call_createForegroundProcess
global call_exit
GLOBAL call_sem_create
GLOBAL call_sem_open
GLOBAL call_sem_close
GLOBAL call_sem_post
GLOBAL call_sem_wait
GLOBAL call_pipe_close
GLOBAL call_pipe_open
GLOBAL call_pipe_read
GLOBAL call_pipe_write

GLOBAL call_kill
GLOBAL call_yield
GLOBAL call_changePriority
GLOBAL call_ps 
GLOBAL call_block
GLOBAL call_unblock 

GLOBAL call_printProcessInfo

GLOBAL call_sleep

GLOBAL call_waitKids
GLOBAL call_begin_gameplay
GLOBAL call_end_gameplay

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
    call_to_handler 45
call_mem_state:
    call_to_handler 61
call_createProcess:
    call_to_handler 40
call_getPid:
    call_to_handler 41
call_createBackgroundProcess:
    call_to_handler 42
call_createForegroundProcess:
    call_to_handler 43
call_exit:
    call_to_handler 44

call_sem_open:
    call_to_handler 46
call_sem_close:
    call_to_handler 47
call_sem_create:
    call_to_handler 48
call_sem_post:
    call_to_handler 49
call_sem_wait:
    call_to_handler 50
call_pipe_open:
    call_to_handler 51
call_pipe_close:
    call_to_handler 52
call_pipe_write:
    call_to_handler 53
call_pipe_read:
    call_to_handler 54

call_changePriority:
    call_to_handler 55

call_ps:
  call_to_handler 56
call_kill:
    call_to_handler 57

call_block:
    call_to_handler 58
call_unblock:
    call_to_handler 59
call_yield:
    call_to_handler 60
call_printProcessInfo:
    call_to_handler 62
call_sleep:
    call_to_handler 63
call_printHex:
    call_to_handler 64
call_print_long_long_int:
    call_to_handler 65
call_waitKids:
    call_to_handler 66
call_begin_gameplay:
    call_to_handler 67
call_end_gameplay:
    call_to_handler 68
call_excepDivZero:
    xor rdx,rdx
    div rdx
call_excepInvalidOp:
    mov cr6, rax