
typedef enum { NONE = 0, EATING, HUNGRY, THINKING } PHILOSOPHER_STATE;

#define MAX_QTY 10
#define MIN_QTY 3
#define SEM_ID "Question"
#define MAX_PHILO_BUFFER 3

#define THINK_TIME 2
#define EAT_TIME 3
#define CLEAR_RENDERS 5

#define COMMAND_QUIT 'q'
#define COMMAND_ADD 'a'
#define COMMAND_REMOVE 'r'
#define COMMAND_PS 'p'
#define COMMAND_CLEAR 'c'

#define left(i) (((i) + qtyPhilosophers - 1) % qtyPhilosophers)
#define right(i) (((i) + 1) % qtyPhilosophers)
#define philoSemaphore(i) (SEM_ID + (i) + 1)
#define A_LOVELY_COLOR_FOR_DINING 0xFFFFF
#define SHORT_WAIT 80

#include <UserSyscalls.h>

static uint8_t qtyPhilosophers = 0;
static PHILOSOPHER_STATE philoStates[MAX_QTY];
static int16_t philoPids[MAX_QTY];
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
  if (phSemID == -1)
    return -1;

  for (int i = 0; i < MAX_QTY; i++) {
    philoStates[i] = NONE;
    philoPids[i] = -1;
  }

  for (int i = 0; i < MIN_QTY; i++)
    addPhilosopher(i, phSemID);

  char command = '\0';
  while ((command = call_getChar()) != COMMAND_QUIT) {
    switch (command) {
    case COMMAND_ADD:
      if (qtyPhilosophers < MAX_QTY) {
        if (addPhilosopher(qtyPhilosophers, phSemID) == -1) {
          // printErr("No se pudo agregar un filosofo\n");
            call_drawStringFormatted("Could not add philosopher...\n",
                                     WHITE, BLACK, 2);

        }

      } else {
        // printf("La mesa esta llena\n");
          call_drawStringFormatted("Table is full...\n",
                                   WHITE, BLACK, 2);
      }
            call_wait(SHORT_WAIT);
      call_paintScreen(A_LOVELY_COLOR_FOR_DINING);
      break;
    case COMMAND_REMOVE:
      if (qtyPhilosophers > MIN_QTY)
        removePhilosopher(qtyPhilosophers - 1, phSemID);
      else {
        // printf("Como minimo debe haber %d filosofos para empezar el
        // banquete\n", MIN_QTY);
          call_drawStringFormatted("Table is too empty...\n",
                                   WHITE, BLACK, 2);
      }
            call_wait(SHORT_WAIT);
            call_paintScreen(A_LOVELY_COLOR_FOR_DINING);
      break;
    case COMMAND_CLEAR:
      singleLine = !singleLine;
      call_paintScreen(A_LOVELY_COLOR_FOR_DINING);
      break;
    }
  }

  for (int i = qtyPhilosophers - 1; i >= 0; i--) {
    removePhilosopher(i, phSemID);
  }
  call_sem_close(SEM_ID);
  call_end_gameplay();
}

static void render() {
  if (singleLine){
      call_setXBuffer(0);
      call_setYBuffer(0);
  }

  const static char letters[] = {' ', 'E', '.', '.'};
  uint8_t somethingToWrite = 0;
  for (int i = 0; i < qtyPhilosophers; i++) {
    if (letters[philoStates[i]] != ' ') {
      somethingToWrite = 1;
      // printf("%c%2s", letters[philoStates[i]], "");
        call_setXBuffer(0);
        call_setYBuffer(0);
        call_drawStringFormatted(letters[philoStates[i]] + '\0', WHITE, BLACK, 2);
        call_drawStringFormatted("\n", WHITE, BLACK, 2);
        call_drawStringFormatted(' ' + '\0', WHITE, BLACK, 2);
        call_drawStringFormatted("\n", WHITE, BLACK, 2);
        call_drawStringFormatted(' ' + '\0', WHITE, BLACK , 2);
        call_drawStringFormatted("\n", WHITE, BLACK, 2);

      /*call_drawLetterFromChar(letters[philoStates[i]]);
      call_drawLetterFromChar(' ');
      call_drawLetterFromChar(' ');*/
    }
  }
  if (somethingToWrite) {
      call_drawStringFormatted("\n", BLACK, WHITE, 2);
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
   /* char entering[] = "Adding a philosopher... ";
    int currentsize = call_getSize();
    call_setSize(2);
    call_setXBuffer(0);
    call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
    for (int i = 0; entering[i] != 0; i++) {
        call_drawLetterFromChar(entering[i]);
    }*/
  call_sem_wait(phsemID);
  char philoNumberBuffer[MAX_PHILO_BUFFER] = {0};
    int phId = call_sem_create(0, philoNames[index]);
  if ( phId == -1) {
    call_sem_post(phsemID);
    return -1;
  }

  my_itoa(index, philoNumberBuffer, 10);
  char *params[] = {"philosopher", philoNumberBuffer,phsemID,phId, NULL};
  philoPids[index] =
      call_createBackgroundProcess(philosopher, 1, params, 4);

  if (philoPids[index] != -1) {
    qtyPhilosophers++;
    render();
    call_sem_post(phsemID);
    return 0;
  } else {
    call_sem_close(philoNames[index]);
    call_sem_post(phsemID);
    return -1;
  }
}

static int removePhilosopher(int index, int phsemID) {

  call_sem_wait(phsemID);
  if (philoPids[index] == -1) {
    call_sem_post(phsemID);
    return -1;
  }
  call_kill(philoPids[index]);
  call_waitKids();
  philoPids[index] = -1;
  philoStates[index] = NONE;
  qtyPhilosophers--;

    call_drawStringFormatted("Sale el filosofo ",
                             WHITE, BLACK, 2);

    call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
    call_drawStringFormatted("\n", WHITE, BLACK, 2);



    call_sem_close(philoNames[index]);
  philoPids[index] = -1;
  render();
  call_sem_post(phsemID);
  return 0;
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
  if (philoStates[i] == HUNGRY && philoStates[left(i)] != EATING &&
      philoStates[right(i)] != EATING) {
    philoStates[i] = EATING;
    render();
    call_sem_post(phID);
  }
}
