#include <io.h>
#include <UserSyscalls.h>
#include <test.h>

void test(){
    call_clear();
    call_drawStringFormatted("CATEDRA SCHEDULER TESTS :\n",WHITE,BLACK,2);
    call_drawStringFormatted("START TEST PROCESSES :\n",WHITE,BLACK,2);
    char * psTest[] = {"hola"};
    if(test_processes(0,psTest) == -1){
        call_drawStringFormatted("SCHEDULER TEST FAILED :\n",RED,BLACK,2);
    }else{
        call_sleep(20);
        call_drawStringFormatted("SCHEDULER TEST OK :\n",GREEN,BLACK,2);
    }
    
    call_sleep(20);
    
   /* call_drawRectangle(BLACK,0,0,call_getWidth(), call_getHeight());
        call_setXBuffer(0);
    call_setYBuffer(0);*/
    call_drawStringFormatted("START TEST PRIO :\n",WHITE,BLACK,2);
    test_prio();

    call_drawStringFormatted("FINISH CATEDRA SCHEDULER TESTS :\n",WHITE,BLACK,2);
    call_sleep(50);
    call_exit();
}

