#include <UserSyscalls.h>
#include <user_lib.h>

// Macros
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

// Static variables
static uint8_t qtyPhilosophers = MIN_PHILOS;
static PHILOSOPHER_STATE philoStates[MAX_PHILOS];
static int16_t philoPids[MAX_PHILOS];
static int forks[MAX_PHILOS];
static int stateLock;

// Philosopher names
static char *philoNames[] = {
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

// Utility functions
int my_atoi(const char *str);
void reverse(char *str, int length);
char *my_itoa(int num, char *str, int base);

// Philosopher functions
void philosopher(int argc, char **argv);
void takeForks(int i);
void putForks(int i);
void testPhilo(int i);
void startPhilosopher(int index);
void initializePhilosophers();
void stopPhilosopher(int index);
void renderPhilosophers();
void run_Philosophers(int argc, char **argv);

int my_atoi(const char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return res;
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
    if (str == NULL) {
        return NULL;
    }
    int i = 0;
    int isNegative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';
    reverse(str, i);
    return str;
}

void philosopher(int argc, char **argv) {
    int i = my_atoi(argv[0]);
    call_drawStringFormatted("Philosopher ", WHITE, BLACK, 2);
    call_drawStringFormatted(philoNames[i], WHITE, BLACK, 2);
    call_drawStringFormatted(" entered\n", WHITE, BLACK, 2);

    philoStates[i] = THINKING;

    while (1) {
        call_sleep(THINK_TIME);

        call_sem_wait(stateLock);
        philoStates[i] = HUNGRY;
        testPhilo(i);
        call_sem_post(stateLock);

        takeForks(i);
        call_sleep(EAT_TIME);
        putForks(i);
    }
}

void startPhilosopher(int index) {
    call_drawStringFormatted("Adding philosopher: ", WHITE, BLACK, 2);
    call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
    call_drawStringFormatted("\n", WHITE, BLACK, 2);

    char philoIndexStr[MAX_PHILO_BUFFER];
    my_itoa(index, philoIndexStr, 10);

    char *params[] = {philoIndexStr, NULL};
    philoPids[index] = call_createBackgroundProcess(philosopher, 1, params, 4, 0);
}

void initializePhilosophers() {
    stateLock = call_sem_create(1, "StateLock");
    for (int i = 0; i < MAX_PHILOS; i++) {
        philoStates[i] = NONE;
        philoPids[i] = -1;
        forks[i] = call_sem_create(1, philoNames[i]);
    }

    for (int i = 0; i < MIN_PHILOS; i++) {
        startPhilosopher(i);
    }
}

void stopPhilosopher(int index) {
    call_drawStringFormatted("Removing philosopher: ", WHITE, BLACK, 2);
    call_drawStringFormatted(philoNames[index], WHITE, BLACK, 2);
    call_drawStringFormatted("\n", WHITE, BLACK, 2);

    if (philoPids[index] != -1) {
        call_kill(philoPids[index]);
        philoPids[index] = -1;
    }
}

void run_Philosophers(int argc, char **argv) {
    call_paintScreen(A_LOVELY_COLOR_FOR_DINING);
    call_begin_gameplay();

    int semID = call_sem_create(1, SEM_ID);
    if (semID == -1) {
        call_drawStringFormatted("Failed to create semaphore...\n", WHITE, BLACK, 2);
        return;
    }

    initializePhilosophers();

    char command = '\0';
    while ((command = (char)call_getChar()) != COMMAND_QUIT) {
        switch (command) {
            case COMMAND_ADD:
                if (qtyPhilosophers < MAX_PHILOS) {
                    startPhilosopher(qtyPhilosophers);
                    qtyPhilosophers++;
                } else {
                    call_drawStringFormatted("Table full!\n", WHITE, BLACK, 2);
                }
                renderPhilosophers();
                break;
            case COMMAND_REMOVE:
                if (qtyPhilosophers > MIN_PHILOS) {
                    stopPhilosopher(qtyPhilosophers - 1);
                    qtyPhilosophers--;
                } else {
                    call_drawStringFormatted("Table too empty!\n", WHITE, BLACK, 2);
                }
                renderPhilosophers();
                break;
            default:
                //call_drawStringFormatted("Please enter a command.\n", WHITE, BLACK, 2);
                break;
        }

    }

    for (int i = qtyPhilosophers - 1; i >= 0; i--) {
        stopPhilosopher(i);
    }
    call_sem_close(SEM_ID);
    call_sem_close("StateLock");
    call_end_gameplay();
}

void takeForks(int i) {
    call_sem_wait(forks[i]);
    call_sem_wait(forks[right(i)]);
}

void putForks(int i) {
    call_sem_post(forks[i]);
    call_sem_post(forks[right(i)]);

    call_sem_wait(stateLock);
    philoStates[i] = THINKING;
    testPhilo(left(i));
    testPhilo(right(i));
    call_sem_post(stateLock);
}

void testPhilo(int i) {
    if (philoStates[i] == HUNGRY && philoStates[left(i)] != EATING && philoStates[right(i)] != EATING) {
        philoStates[i] = EATING;
        call_sem_post(forks[i]);
    }
}

void renderPhilosophers() {
    const static char letters[] = {'.', 'E', '.', '.'};
    int yPos = 0;
    call_wait(10);
    call_paintScreen(A_LOVELY_COLOR_FOR_DINING);

    for (int i = 0; i < qtyPhilosophers; i++) {
        call_setXBuffer(0);
        call_setYBuffer(yPos);
        char stateChar = letters[philoStates[i]];
        call_drawStringFormatted(&stateChar, WHITE, BLACK, 2);
        yPos += 16; // Move to the next line based on font size
        call_drawStringFormatted("  ", WHITE, BLACK, 2);
    }
}

