#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <ncurses.h>

/** Globals */
uint32_t top_thread = 0;
bool terminate_thread = false;

/**
 * @brief Thread burner task
 *
 * @param threadId      Thread identifier
 * @return void*
 */
void *count_up(void *threadId) {
    unsigned long int val = 0;
    int this_thread = *(int *)threadId;
    while (true) {
        val += 1;
        if (val % 1000000 == 0
                && this_thread == top_thread
                && terminate_thread == true) {
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    char cmd;
    int thread_status;
    const uint32_t nproc = sysconf(_SC_NPROCESSORS_ONLN);
    const uint32_t MAX_THREADS = 10 * nproc;
    pthread_t threads[MAX_THREADS];

    /** Screen init */
    initscr();
    cbreak();
    noecho();

    while (true) {
        printw("# Processors:    %d\n", nproc);
        printw("Avtive threads:  %d\n", top_thread);
        printw("j(up) k(down) q(uit)\n");
        refresh();

        cmd = getch();

        if (cmd == 'j' && top_thread < MAX_THREADS) {
            top_thread++;
            thread_status = pthread_create(&threads[top_thread], NULL, count_up, &top_thread);
            if (thread_status) {
                printf("Error: return code from pthread_create() is %d\n", thread_status);
                goto out;
            }
        }

        if (cmd == 'k' && top_thread > 0) {
            terminate_thread = true;
            pthread_join(threads[top_thread], NULL);
            terminate_thread = false;
            top_thread--;
        }

        if (cmd == 'q') {
            printf("Exiting...\n");
            goto out;
        }
        clear();
    }

out:
    endwin();
    return 0;
}
