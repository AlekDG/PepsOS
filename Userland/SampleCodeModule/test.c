#include <io.h>
#include <UserSyscalls.h>
#include <test.h>

void test(){
    call_clear();
    char * psTest[] = {"hola"};
    test_processes(0,psTest);
    call_sleep(5);
    
    call_drawRectangle(BLACK,0,0,call_getWidth(), call_getHeight());
        call_setXBuffer(0);
    call_setYBuffer(0);
    call_drawStringFormatted("TEST PRIO :\n",WHITE,BLACK,2);
    test_prio();

    call_drawStringFormatted("FINISH CATEDRA TESTS :\n",WHITE,BLACK,2);
    call_sleep(5);
    char * mmTest[] = {"128"};
    test_mm(1, mmTest);
    call_exit();
}

