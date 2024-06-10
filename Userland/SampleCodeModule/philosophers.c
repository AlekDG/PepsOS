
typedef enum { NONE = 0,
    EATING,
    HUNGRY,
    THINKING } PHILOSOPHER_STATE;

#define MAX_QTY 10
#define MIN_QTY 3
#define SEM_ID 42
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

#include <UserSyscalls.h>

static uint8_t qtyPhilosophers = 0;
static PHILOSOPHER_STATE philoStates[MAX_QTY];
static int16_t philoPids[MAX_QTY];
static uint8_t singleLine = 0;


static void render();
static int philosopher(int argc, char **argv);
static int addPhilosopher(int index);
static int removePhilosopher(int index);
static void takeForks(int i);
static void putForks(int i);
static void test(int i);

static const char *philoNames[] = {
        "Montesqueso", "Fridge", "Spinetta", "Gillotina",
        "Chasquibum", "Triple", "Milldeck", "Key",
        "Ancla Scantron", "Descartado", "Confundido", "Plato",
        "Panacea", "Planeta Enano", "El Filosofo Que No Debe Ser Nombrado", "Heidi",
        "Aquiles", "Bungee", "Sastre", "Espinosa",
        "de Beaver", "Penduloso", "Leipzig", "Growl",
        "Calvin", "Arkham", "Witty", "Kindergarde",
        "Canto", "Teapot", "Artistofanes", "Caramel Marks"};


int my_atoi(const char *str) {
    int result = 0;
    int sign = 1;

    // Skip whitespaces
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' || *str == '\f' || *str == '\v') {
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

char* my_itoa(int num, char *str, int base) {
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

void runPhilosophers(int argc, char **argv) {
    if (call_sem_create(1, SEM_ID) == -1)
        return -1;

    for (int i = 0; i < MAX_QTY; i++) {
        philoStates[i] = NONE;
        philoPids[i] = -1;
    }

    for (int i = 0; i < MIN_QTY; i++)
        addPhilosopher(i);

    char command = '\0';
    while ((command = call_getChar()) != COMMAND_QUIT) {
        switch (command) {
            case COMMAND_ADD:
                if (qtyPhilosophers < MAX_QTY) {
                    if (addPhilosopher(qtyPhilosophers) == -1){
                        //printErr("No se pudo agregar un filosofo\n");
                        char couldNotAdd[] = "Could not add philosopher\n";
                        int currentsize = call_getSize();
                        call_setSize(2);
                        call_setXBuffer(0);
                        call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
                        for (int i = 0; couldNotAdd[i] != 0; i++)
                        {
                            call_drawLetterFromChar(couldNotAdd[i]);
                        }
                    }

                }
                else {
                   // printf("La mesa esta llena\n");
                    char tableFull[] = "Table is full\n";
                    int currentsize = call_getSize();
                    call_setSize(2);
                    call_setXBuffer(0);
                    call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
                    for (int i = 0; tableFull[i] != 0; i++)
                    {
                        call_drawLetterFromChar(tableFull[i]);
                    }
                }
                break;
            case COMMAND_REMOVE:
                if (qtyPhilosophers > MIN_QTY)
                    removePhilosopher(qtyPhilosophers - 1);
                else {
                   // printf("Como minimo debe haber %d filosofos para empezar el banquete\n", MIN_QTY);
                    char tableEmpty[] = "Table is too empty, need more philosophers\n";
                    int currentsize = call_getSize();
                    call_setSize(2);
                    call_setXBuffer(0);
                    call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
                    for (int i = 0; tableEmpty[i] != 0; i++)
                    {
                        call_drawLetterFromChar(tableEmpty[i]);
                    }
                }
                break;
            case COMMAND_CLEAR:
                singleLine = !singleLine;
                break;
        }
    }

    for (int i = qtyPhilosophers - 1; i >= 0; i--) {
        removePhilosopher(i);
    }
    call_sem_close(SEM_ID);
}


static void render() {
    if (singleLine)
        call_paintScreen(0xFFFFFF);
    const static char letters[] = {' ', 'E', '.', '.'};
    uint8_t somethingToWrite = 0;
    for (int i = 0; i < qtyPhilosophers; i++) {
        if (letters[philoStates[i]] != ' ') {
            somethingToWrite = 1;
            //printf("%c%2s", letters[philoStates[i]], "");

            call_drawLetterFromChar(letters[philoStates[i]]);
            call_drawLetterFromChar(' ');
            call_drawLetterFromChar(' ');
        }
    }
    if (somethingToWrite)
        call_drawLetterFromChar('\n');
}

static int philosopher(int argc, char **argv) {
    int i = my_atoi(argv[1]);
    char entering[] = "Entra el filosofo: ";
    int currentsize = call_getSize();
    call_setSize(2);
    call_setXBuffer(0);
    call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
    for (int i = 0; entering[i] != 0; i++)
    {
        call_drawLetterFromChar(entering[i]);
    }
    for(int k = 0; philoNames[i][k] != 0; k++){
        call_drawLetterFromChar(philoNames[i][k]);
    }
    call_setSize(currentsize);
    philoStates[i] = THINKING;
    while (1) {
        call_sleep(THINK_TIME);
        takeForks(i);
        call_sleep(EAT_TIME);
        putForks(i);
    }
}

static int addPhilosopher(int index) {
    call_sem_wait(SEM_ID);
    char philoNumberBuffer[MAX_PHILO_BUFFER] = {0};
    if (call_sem_create(philoSemaphore(index), 0) == -1)
        return -1;
    my_itoa(index, philoNumberBuffer, 10);
    char *params[] = {"philosopher", philoNumberBuffer, NULL};
    //philoPids[index] = createProcessWithFds(&philosopher, params, "philosopher", 4, fileDescriptors);
    call_createProcess(&philosopher, params, "philosopher", 4);
    if (philoPids[index] != -1)
        qtyPhilosophers++;
    render();
    call_sem_post(SEM_ID);
    return -1 * !(philoPids[index] + 1);
}

static int removePhilosopher(int index) {

    char exiting[] = "Queremos que salga el filosofo: ";
    int currentsize = call_getSize();
    call_setSize(2);
    call_setXBuffer(0);
    call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
    for (int i = 0; exiting[i] != 0; i++)
    {
        call_drawLetterFromChar(exiting[i]);
    }
    for(int k = 0; philoNames[index][k] != 0; k++){
        call_drawLetterFromChar(philoNames[index][k]);
    }
    call_setSize(currentsize);

    call_sem_wait(SEM_ID);
    while (philoStates[left(index)] == EATING && philoStates[right(index)] == EATING) { // Estado invalido para remover
        call_sem_post(SEM_ID);
        call_sem_wait(philoSemaphore(index)); // Bloquearse hasta que el filosofo este comiendo
        call_sem_wait(SEM_ID);
    }
    call_kill(philoPids[index]);
    call_waitKids(philoPids[index]);

    char leaving[] = "Sale el filosofo: ";
    currentsize = call_getSize();
    call_setSize(2);
    call_setXBuffer(0);
    call_setYBuffer(2 * (call_getHeight() / 3) + 13 * call_getSize());
    for (int i = 0; leaving[i] != 0; i++)
    {
        call_drawLetterFromChar(leaving[i]);
    }
    for(int k = 0; philoNames[index][k] != 0; k++){
        call_drawLetterFromChar(philoNames[index][k]);
    }
    call_setSize(currentsize);

    call_sem_close(philoSemaphore(index));
    philoPids[index] = -1;
    philoStates[index] = NONE;
    qtyPhilosophers--;
    render();
    call_sem_post(SEM_ID);
    return 0;
}

static void takeForks(int i) {
    call_sem_wait(SEM_ID);
    philoStates[i] = HUNGRY;
    test(i);
    call_sem_post(SEM_ID);
    call_sem_wait(philoSemaphore(i));
}

static void putForks(int i) {
    call_sem_wait(SEM_ID);
    philoStates[i] = THINKING;
    render();
    test(left(i));
    test(right(i));
    call_sem_post(SEM_ID);
}

static void test(int i) {
    if (philoStates[i] == HUNGRY && philoStates[left(i)] != EATING && philoStates[right(i)] != EATING) {
        philoStates[i] = EATING;
        call_sem_post(philoSemaphore(i));
        render();
    }
}

