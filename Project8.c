#include <stdio.h>

#define MAX_FRAMES 100
#define MAX_REFERENCES 100
#define EMPTY_FRAME -1

int isInFrame(int frame[], int size, int page) {
    for (int i = 0; i < size; i++) {
        if (frame[i] == page)
            return 1;
    }
    return 0;
}

void fifo(int pages[], int n, int frameSize) {
    int frame[MAX_FRAMES];
    int pageFaults = 0;
    int index = 0;

    for (int i = 0; i < frameSize; i++) {
        frame[i] = EMPTY_FRAME;
    }

    printf("\nFIFO Simulation\n");
    for (int i = 0; i < n; i++) {
        printf("Step %2d: Page %2d -> [", i + 1, pages[i]);


        if (!isInFrame(frame, frameSize, pages[i])) {
            frame[index] = pages[i];
            index = (index + 1) % frameSize;
            pageFaults++;
        }

        for (int j = 0; j < frameSize; j++) {
            if (frame[j] == EMPTY_FRAME) printf(" -");
            else printf("%2d", frame[j]);
            if (j != frameSize - 1) printf(" ");
        }
        printf("]\n");
    }

    printf("Total Page Faults (FIFO): %d\n", pageFaults);
}

int findLRU(int time[], int n) {
    int min = time[0], pos = 0;
    for (int i = 1; i < n; ++i) {
        if (time[i] < min) {
            min = time[i];
            pos = i;
        }
    }
    return pos;
}

void lru(int pages[], int n, int framesCount) {
    int frames[MAX_FRAMES], time[MAX_FRAMES];
    int pageFaults = 0, timeCounter = 0;

    for (int i = 0; i < framesCount; ++i) {
        frames[i] = EMPTY_FRAME;
    }

    printf("\nLRU Simulation\n");
    for (int i = 0; i < n; ++i) {
        int pageFound = 0;

        for (int j = 0; j < framesCount; ++j) {
            if (frames[j] == pages[i]) {
                timeCounter++;
                time[j] = timeCounter;
                pageFound = 1;
                break;
            }
        }

        if (!pageFound) {
            int pos = -1;
            for (int j = 0; j < framesCount; ++j) {
                if (frames[j] == EMPTY_FRAME) {
                    pos = j;
                    break;
                }
            }

            if (pos == -1) {
                pos = findLRU(time, framesCount);
            }

            frames[pos] = pages[i];
            timeCounter++;
            time[pos] = timeCounter;
            pageFaults++;
        }

        printf("Step %2d: Page %2d -> [", i + 1, pages[i]);
        for (int j = 0; j < framesCount; ++j) {
            if (frames[j] == EMPTY_FRAME) printf(" -");
            else printf("%2d", frames[j]);
            if (j != framesCount - 1) printf(" ");
        }
        printf("]\n");
    }

    printf("Total Page Faults (LRU): %d\n", pageFaults);
}

int findOptimal(int pages[], int frames[], int n, int framesCount, int index) {
    int pos = -1, farthest = index;

    for (int i = 0; i < framesCount; ++i) {
        int j;
        for (j = index; j < n; ++j) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }

        if (j == n) {
            return i;
        }
    }

    return (pos == -1) ? 0 : pos;
}

void optimal(int pages[], int n, int framesCount) {
    int frames[MAX_FRAMES];
    int pageFaults = 0;

    for (int i = 0; i < framesCount; ++i)
        frames[i] = EMPTY_FRAME;

    printf("\nOptimal Simulation\n");
    for (int i = 0; i < n; ++i) {
        int pageFound = 0;

        for (int j = 0; j < framesCount; ++j) {
            if (frames[j] == pages[i]) {
                pageFound = 1;
                break;
            }
        }

        if (!pageFound) {
            int pos = -1;
            for (int j = 0; j < framesCount; ++j) {
                if (frames[j] == EMPTY_FRAME) {
                    pos = j;
                    break;
                }
            }

            if (pos == -1) {
                pos = findOptimal(pages, frames, n, framesCount, i + 1);
            }

            frames[pos] = pages[i];
            pageFaults++;
        }

        printf("Step %2d: Page %2d -> [", i + 1, pages[i]);
        for (int j = 0; j < framesCount; ++j) {
            if (frames[j] == EMPTY_FRAME) printf(" -");
            else printf("%2d", frames[j]);
            if (j != framesCount - 1) printf(" ");
        }
        printf("]\n");
    }

    printf("Total Page Faults (Optimal): %d\n", pageFaults);
}

int main() {
    int pages[MAX_REFERENCES];
    int n, framesCount, choice;
    char continueChoice;

    while (1) {
        printf("Enter number of page references (30–100): ");
        if (scanf("%d", &n) == 1 && n >= 30 && n <= MAX_REFERENCES) break;
        else {
            printf("Error! Number of page references should be from 30 to 100.\n");
            while (getchar() != '\n');
        }
    }

    while (1) {
        int valid = 1;
        printf("Enter the page reference sequence:\n");
        for (int i = 0; i < n; ++i) {
            if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) {
                valid = 0;
                printf("Error: Invalid page reference. Start over.\n");
                while (getchar() != '\n');
                break;
            }
        }
        if (valid) break;
    }

    while (1) {
        printf("Enter number of frames (1-10): ");
        if (scanf("%d", &framesCount) == 1 && framesCount >= 1 && framesCount <= 10) break;
        else {
            printf("Error! Number of frames should be from 1 to 10.\n");
            while (getchar() != '\n');
        }
    }

    do {
        while (1) {
            printf("\nPick an algorithm:\n");
            printf("1. FIFO\n");
            printf("2. LRU\n");
            printf("3. Optimal\n");
            printf("4. Exit\n");
            printf("What one would you like to use? ");
            if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 4) break;
            else {
                printf("Choice out of bounds, choose 1–4.\n");
                while (getchar() != '\n');
            }
        }

        switch (choice) {
            case 1:
                fifo(pages, n, framesCount);
                break;
            case 2:
                lru(pages, n, framesCount);
                break;
            case 3:
                optimal(pages, n, framesCount);
                break;
            case 4:
                printf("Exiting\n");
                return 0;
        }

        while (1) {
            printf("\nDo you want to try another algorithm? (Y/N): ");
            scanf(" %c", &continueChoice);
            if (continueChoice == 'Y' || continueChoice == 'y' || continueChoice == 'N' || continueChoice == 'n')
                break;
            else {
                printf("Error! Enter Y or N.\n");
                while (getchar() != '\n');
            }
        }

        if (continueChoice == 'N' || continueChoice == 'n') {
            printf("Thanks for playing!\n");
            break;
        }

    } while (1);

    return 0;
}
