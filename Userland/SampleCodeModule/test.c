#include <UserSyscalls.h>
#include <test.h>

void test(){
    call_drawRectangle(BLACK,0,0,call_getWidth(), call_getHeight());
    call_setXBuffer(0);
    call_setYBuffer(0);
    //call_drawStringFormatted("TEST PROC :\n",WHITE,BLACK,2);
    test_processes(0,"hola");
    //    call_setXBuffer(0);
    //call_setYBuffer(0);
    //call_drawStringFormatted("OK :\n",WHITE,BLACK,2);
    call_sleep(5);
    
    call_drawRectangle(BLACK,0,0,call_getWidth(), call_getHeight());
        call_setXBuffer(0);
    call_setYBuffer(0);
    call_drawStringFormatted("TEST PRIO :\n",WHITE,BLACK,2);
    test_prio();

    call_drawStringFormatted("FINISH CATEDRA TESTS :\n",WHITE,BLACK,2);
    call_sleep(5);
    test_mm(1, "test_mm");
    call_exit();
}

