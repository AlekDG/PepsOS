#include <UserSyscalls.h>
#include <io.h>
#include <test.h>
#include <user_lib.h>

void testShceduler(int argc, char *argv[]) {
  call_clear();
  if (argc != 1) {
    call_drawStringFormatted("ERROR: cantidad de argmentos incorrecta\n", RED,
                             BLACK, 2);
    call_drawStringFormatted("PRESIONE UNA TECLA PARA SALIR \n", WHITE, BLACK,
                             2);
    call_pipe_read(STDIN);
    call_exit();
  }
  int maxProcesses = satoi(argv[1]);
  if(maxProcesses > 45){
    call_drawStringFormatted("ERROR: cantidad de procesos a creae no puede ser superior a 45\n", RED,
                             BLACK, 2);
    call_drawStringFormatted("PRESIONE UNA TECLA PARA SALIR \n", WHITE, BLACK,
                             2);
    call_pipe_read(STDIN);
    call_exit();

  }
  call_drawStringFormatted("CATEDRA SCHEDULER TESTS :\n", WHITE, BLACK, 2);
  call_drawStringFormatted("START TEST PROCESSES :\n", WHITE, BLACK, 2);
  
  if (maxProcesses <= 0) {
    call_drawStringFormatted(
        "ERROR: Debe ingresar un numero valido como argumento\n", RED, BLACK,
        2);
    call_exit();
  }
  if (test_processes(maxProcesses) == -1) {
    call_drawStringFormatted("SCHEDULER TEST FAILED :\n", RED, BLACK, 2);
  } else {
    call_sleep(20);
    call_drawStringFormatted("SCHEDULER TEST OK :\n", GREEN, BLACK, 2);
  }

  call_sleep(20);

  call_drawStringFormatted("START TEST PRIO :\n", WHITE, BLACK, 2);
  test_prio();

  call_drawStringFormatted(
      "FINISH CATEDRA SCHEDULER TESTS PRESIONE UNA TECLA PARA SALIR :\n", WHITE,
      BLACK, 2);
  call_pipe_read(STDIN);
  call_exit();
}
