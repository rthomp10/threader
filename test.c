#include <stdio.h>
#include <pthread.h>
#include <conio.h>
#include <windows.h>

typedef enum { false, true } bool;  // Define a bool type with values false and true
unsigned int top_thread;
bool terminate;

void *count_up(void *threadId) {
    unsigned long int val = 0;
    int this_thread = *(int *)threadId;

    while (1) {
        val += 1;
        if (val % 1000000 == 0 && this_thread == top_thread && terminate == true){
            break;
        }
    }
    pthread_exit(NULL);
}


int main() {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    printf("nproc: %u\n", sys_info.dwNumberOfProcessors);
    const long MAX_THREADS = 10*sys_info.dwNumberOfProcessors;
    pthread_t threads[MAX_THREADS];
    int rc;
    char ch;
    top_thread = 0;
    terminate = false;

    while (1) {
        system("cls");
        printf("Avtive threads: %d\n", top_thread);
        printf("j(up) k(down) q(uit)\n");

        ch = _getch();

        if (ch == 'j' && top_thread < MAX_THREADS) {
            top_thread++;
            rc = pthread_create(&threads[top_thread], NULL, count_up, &top_thread);
            if (rc) {
                printf("Error: return code from pthread_create() is %d\n", rc);
                return -1;
            }
        } else if (ch == 'k' && top_thread > 0) {
            terminate = true;
            pthread_join(threads[top_thread], NULL);
            terminate = false;
            top_thread--;
        } else if (ch == 'q') {
            printf("Exiting...\n");
            break;
        }
    }

    return 0;
}

