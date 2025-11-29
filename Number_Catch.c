#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// for colors in CMD
void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int score = 0;
int lives = 3;
int timeLimit = 350;     // default speed
int highScore = 0;

// to load high score
void loadHighScore() {
    FILE *f = fopen("highscore.txt", "r");
    if (f) {
        fscanf(f, "%d", &highScore);
        fclose(f);
    } else {
        highScore = 0;
    }
}

// save high score
void saveHighScore() {
    if (score > highScore) {
        FILE *f = fopen("highscore.txt", "w");
        fprintf(f, "%d", score);
        fclose(f);
    }
}

// reverse mode flag (random)
int reverseMode = 0;

// generate random number
int getRand() {
    return (rand() % 9) + 1;
}

// draw progress bar
void showBar(int t, int max) {
    int totalBars = 20;
    int filled = (t * totalBars) / max;

    printf("[");
    for (int i = 0; i < filled; i++) printf("#");
    for (int i = filled; i < totalBars; i++) printf("-");
    printf("]");
}

// prints the UI
void drawUI(int num) {
    system("cls");

    color(11);
    printf("Number Catch Game (Assignment 3)\n");
    color(7);

    printf("Score: %d    Lives: %d    High Score: %d\n\n", score, lives, highScore);

    if (reverseMode == 0) {
        printf("Catch this number:\n\n");
        color(10);
        printf("            [%d]\n\n", num);
        color(7);
        printf("Press the same number quickly!\n\n");
    } else {
        printf("Reverse Mode Active!\n\n");
        color(12);
        printf("       DO NOT PRESS [%d]\n\n", num);
        color(7);
    }
}

int main() {

    int num, ch;
    int timePassed;

    srand(time(NULL));

    loadHighScore();

    // -------- Difficulty Menu --------
    system("cls");
    printf("Select Difficulty:\n\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n\n");
    printf("Enter choice: ");

    char choice = getch();
    if (choice == '1') timeLimit = 450;
    else if (choice == '2') timeLimit = 350;
    else timeLimit = 250;

    // game loop
    while (1) {

        num = getRand();
        reverseMode = (rand() % 7 == 0);  // rare chance of reverse mode

        timePassed = 0;

        while (1) {

            drawUI(num);
            printf("Time Left: ");
            showBar(timePassed, timeLimit);
            printf("\n\n(press q to quit)\n");

            // check input
            if (kbhit()) {
                ch = getch();

                if (ch == 'q') {
                    saveHighScore();
                    system("cls");
                    printf("You quit the game.\n");
                    return 0;
                }

                int pressed = ch - '0';

                // REVERSE MODE LOGIC
                if (reverseMode) {
                    if (pressed != num) {
                        score++;
                        color(10);
                        Beep(1200, 80);
                        color(7);
                    } else {
                        lives--;
                        color(12);
                        Beep(300, 200);
                        color(7);
                    }
                }

                // NORMAL MODE
                else {
                    if (pressed == num) {
                        score++;
                        color(10);
                        Beep(900, 60);
                        color(7);
                    } else {
                        lives--;
                        color(12);
                        Beep(300, 200);
                        color(7);
                    }
                }

                break;
            }

            Sleep(15);
            timePassed += 15;

            // ran out of time
            if (timePassed >= timeLimit) {
                lives--;
                color(12);
                Beep(200, 200);
                color(7);
                break;
            }
        }

        // increase difficulty slowly
        if (score % 5 == 0 && timeLimit > 120) {
            timeLimit -= 10;
        }

        if (lives <= 0) {
            saveHighScore();
            system("cls");
            printf("\nGame Over!\n");
            printf("Final Score: %d\n", score);
            printf("High Score: %d\n", highScore);
            Beep(200, 400);
            break;
        }
    }

    return 0;
}
