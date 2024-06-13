#include <UserSyscalls.h>
#include <user_lib.h>

#define MAX_PHILOS 10
#define MIN_PHILOS 3
#define SEM_ID "Question"
#define MAX_PHILO_BUFFER 3

#define THINK_TIME 2
#define EAT_TIME 3


#define COMMAND_ADD 'a'
#define COMMAND_REMOVE 'r'
#define COMMAND_QUIT 'q'


#define left(i) (((i) + qtyPhilosophers - 1) % qtyPhilosophers)
#define right(i) (((i) + 1) % qtyPhilosophers)
#define A_LOVELY_COLOR_FOR_DINING 0xFFFFF
#define SHORT_WAIT 800

typedef enum { NONE = 0, EATING, HUNGRY, THINKING } PHILOSOPHER_STATE;

static uint8_t qtyPhilosophers = 0;
static PHILOSOPHER_STATE philoStates[MAX_PHILOS];
static int16_t philoPids[MAX_PHILOS];
static uint8_t singleLine = 0;

static void render();
static int philosopher(int argc, char **argv);
static int addPhilosopher(int index, int phsemID);
static int removePhilosopher(int index, int phsemID);
static void takeForks(int i, int phsmID, int phID);
static void putForks(int i, int phsmID, int phID);
static void test(int i, int phID);

static const char *philoNames[] = {
    "Montesqueso",  "Fridge",        "Spinetta",
    "Gillotina",    "Chasquibum",    "Triple",
    "Milldeck",     "Key",           "Ancla Scantron",
    "Descartado",   "Confundido",    "Plato",
    "Panacea",      "Planeta Enano", "El Filosofo Que No Debe Ser Nombrado",
    "Heidi",        "Aquiles",       "Bungee",
    "Sastre",       "Espinosa",      "de Beaver",
    "Penduloso",    "Leipzig",       "Growl",
    "Calvin",       "Arkham",        "Witty",
    "Kindergarde",  "Canto",         "Teapot",
    "Artistofanes", "Caramel Marks"};

int my_atoi(const char *str) {
  int result = 0;
  int sign = 1;

  // Skip whitespaces
  while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' ||
         *str == '\f' || *str == '\v') {
    str++;
  }

  // Check for sign
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }

  // Convert digits to integer
  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return result * sign;
}

void reverse(char *str, int length) {
  int start = 0;
  int end = length - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}

char *my_itoa(int num, char *str, int base) {
  int i = 0;
  int isNegative = 0;

  // Handle 0 explicitly, otherwise empty string is printed for 0
  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  // In standard itoa(), negative numbers are handled only with base 10
  // Otherwise, numbers are considered unsigned
  if (num < 0 && base == 10) {
    isNegative = 1;
    num = -num;
  }

  // Process individual digits
  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  // If number is negative, append '-'
  if (isNegative) {
    str[i++] = '-';
  }

  str[i] = '\0'; // Append string terminator

  // Reverse the string
  reverse(str, i);

  return str;
}

void run_Philosophers(int argc, char **argv) {
    call_paintScreen(A_LOVELY_COLOR_FOR_DINING);
    call_begin_gameplay();

    int phSemID = call_sem_create(1, SEM_ID);
    if (phSemID == -1) {
        call_drawStringFormatted("No se pudo crear el semaforo...\n", WHITE, BLACK, 2);
        return;
    }

    for (int i = 0; i < MAX_PHILOS; i++) {
        philoStates[i] = NONE;
        philoPids[i] = -1;
    }

    for (int i = 0; i < MIN_PHILOS; i++) {
        if (addPhilosopher(i, phSemID) == -1) {
            call_drawStringFormatted("No se pudieron crear los filosofos iniciales...\n", WHITE, BLACK, 2);
        }
        call_wait(SHORT_WAIT);
    }

    char command = '\0';
    while ((command = call_getChar()) != COMMAND_QUIT) {
        switch (command) {
            case COMMAND_ADD:
                if (qtyPhilosophers < MAX_PHILOS) {
                    if (addPhilosopher(qtyPhilosophers, phSemID) == -1) {
                        call_drawStringFormatted("No se pudo agregar filosofo...\n", WHITE, BLACK, 2);
                    }
                } else {
                    call_drawStringFormatted("Mesa llena!\n", WHITE, BLACK, 2);
                }
                call_wait(SHORT_WAIT);
                break;
            case COMMAND_REMOVE:
                if (qtyPhilosophers > MIN_PHILOS) {
                    if (removePhilosopher(qtyPhilosophers - 1, phSemID) == -1) {
                        call_drawStringFormatted("No se pudo remover filosofo...\n", WHITE, BLACK, 2);
                    }
                } else {
                    call_drawStringFormatted("Mesa muy vacia!\n", WHITE, BLACK, 2);
                }
                call_wait(SHORT_WAIT);
                break;
            default:

                call_drawStringFormatted("COMANDO DESCONOCIDO E INESCRUTABLE\n", WHITE, BLACK, 2);
                break;

        }
       // render();
    }

    for (int i = qtyPhilosophers - 1; i >= 0; i--) {
        removePhilosopher(i, phSemID);
    }
    call_sem_close(SEM_ID);
    call_end_gameplay();
}


static void render() {
    const static char letters[] = {' ', 'E', '.', '.'};
    int yPos = 0;


    call_paintScreen(A_LOVELY_COLOR_FOR_DINING);

    for (int i = 0; i < qtyPhilosophers; i++) {

        call_setXBuffer(0);
        call_setYBuffer(yPos);
        call_drawStringFormatted(&letters[philoStates[i]], WHITE, BLACK, 2);
        yPos += 16; // Move to the next line based on font size
        call_drawStringFormatted("  ", WHITE, BLACK, 2);
    }
}




static int philosopher(int argc, char **argv) {
  int i = my_atoi(argv[1]);
    call_drawStringFormatted("Entra el filosofo ",
                             WHITE, BLACK, 2);

    call_drawStringFormatted(philoNames[i], WHITE, BLACK, 2);
    call_drawStringFormatted("\n", WHITE, BLACK, 2);


    philoStates[i] = THINKING;
  while (1) {
    call_sleep(THINK_TIME);
    takeForks(i, argv[2], argv[3]);
    call_sleep(EAT_TIME);
    putForks(i, argv[2], argv[3]);
  }
}

static int addPhilosopher(int index, int phsemID) {
    call_sem_wait(phsemID);

    call_drawStringFormatted("Sumando filosofo: ", WHITE, BLACK, 2);
    call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
    call_drawStringFormatted("\n", WHITE, BLACK, 2);

    char philoNumberBuffer[MAX_PHILO_BUFFER] = {0};
    int phId = call_sem_create(0, philoNames[index]);
    if (phId == -1) {
        call_drawStringFormatted("No se pudo crear el semaforo...\n", WHITE, BLACK, 2);
        call_sem_post(phsemID);
        return -1;
    }

    my_itoa(index, philoNumberBuffer, 10);
    char *params[] = {"philosopher", philoNumberBuffer, phsemID, phId, NULL};
    philoPids[index] = call_createBackgroundProcess(philosopher, 1, params, 4);

    if (philoPids[index] != -1) {
        qtyPhilosophers++;
        call_sem_post(phsemID);
        return 0;
    } else {
        call_drawStringFormatted("No se pudo crear el proceso filosofo...\n", WHITE, BLACK, 2);
        call_sem_close(philoNames[index]);
        call_sem_post(phsemID);
        return -1;
    }
}


static void takeForks(int i, int phsemID, int phID) {
    call_sem_wait(phsemID);
    philoStates[i] = HUNGRY;
    test(i, phID);
    call_sem_post(phsemID);
    call_sem_wait(phID);
}

static void putForks(int i, int phsemID, int phID) {
    call_sem_wait(phsemID);
    philoStates[i] = THINKING;
    render();
    test(left(i), phID);
    test(right(i), phID);
    call_sem_post(phsemID);
}

static void test(int i, int phID) {
    if (philoStates[i] == HUNGRY && philoStates[left(i)] != EATING && philoStates[right(i)] != EATING) {
        philoStates[i] = EATING;
        render();
        call_sem_post(phID);
    }
}

static int removePhilosopher(int index, int phsemID) {
    call_sem_wait(phsemID);

    if (philoPids[index] == -1) {
        call_drawStringFormatted("No existe el filosofo!\n", WHITE, BLACK, 2);
        call_sem_post(phsemID);
        return -1;
    }

    call_drawStringFormatted("Removiendo: ", WHITE, BLACK, 2);
    call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
    call_drawStringFormatted("\n", WHITE, BLACK, 2);

    if (call_kill(philoPids[index]) == -1) {
        call_drawStringFormatted("El filosofo se rehusa a morir.\n", WHITE, BLACK, 2);
        call_sem_post(phsemID);
        return -1;
    }

    call_waitKids();
    philoPids[index] = -1;
    philoStates[index] = NONE;
    qtyPhilosophers--;

    if (call_sem_close(philoNames[index]) == -1) {
        call_drawStringFormatted("No se pudo cerrar el semaforo...", WHITE, BLACK, 2);
        call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
        call_drawStringFormatted("\n", WHITE, BLACK, 2);
    } else {
        call_drawStringFormatted("Cerrado el semaforo de: ", WHITE, BLACK, 2);
        call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
        call_drawStringFormatted("\n", WHITE, BLACK, 2);
    }
    call_sem_post(phsemID);
    return 0;
}


