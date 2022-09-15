#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#define W 80
#define H 25

void game_of_life(void);
int check_rules(int prev_field[H][W], int next_field[H][W], int i, int j, int state_checker);
void print_field(int prev_field[H][W]);
void generate(int prev_field[H][W]);
void set_keypress(void);
void reset_keypress(void);
static struct termios stored_settings;

int main() {
    game_of_life();
    return 0;
}

void game_of_life() {
    int prev_field[H][W] = {0}, next_field[H][W] = {0};
    int state_checker = 0, speed = 250000;
    generate(prev_field);
    print_field(prev_field);
    usleep(100000);
    system("clear");
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        printf(" ");
    }
    while (state_checker == 0) {
        char control;
        fd_set rfds;
        struct timeval tv;
        int retval;
        set_keypress();
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        usleep(40000);
         retval = select(2, &rfds, NULL, NULL, &tv);
        if (retval) {
            control = getc(stdin);
            if (control == 'g') {
                speed -= 10000;
            } else if (control == 't') {
                speed += 10000;
            } else if (control == 27) {
                _Exit(0);
            }
            reset_keypress();
        }
        if (speed < 0) speed = 0;
        if (speed > 1000000) speed = 1000000;
        state_checker = 0;
        for (int i = 0; i < H; i++) {
            for (int j = 1; j < W; j++) {
                check_rules(prev_field, next_field, i, j, state_checker);
            }
        }
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                prev_field[i][j] = next_field[i][j];
            }
        }
        print_field(prev_field);
        if (speed < 0)
            speed = 0;
        usleep(speed);
    }
}

void print_field(int prev_field[H][W]) {
    system("clear");
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (prev_field[i][j] == 1)
                printf("*");
            else
                printf(" ");
        }
        printf("\n");
    }
}

int check_rules(int prev_field[H][W], int next_field[H][W], int i, int j, int state_checker) {
    int fam_count = 0;
    if (prev_field[(i - 1 + H) % H][(j - 1 + W) % W] == 1)
        fam_count++;
    if (prev_field[(i - 1 + H) % H][(j + 1 + W) % W] == 1)
        fam_count++;
    if (prev_field[(i - 1 + H) % H][j] == 1)
        fam_count++;
    if (prev_field[(i + 1 + H) % H][(j - 1 + W) % W] == 1)
        fam_count++;
    if (prev_field[(i + 1 + H) % H][(j + 1 + W) % W] == 1)
        fam_count++;
    if (prev_field[(i + 1 + H) % H][j] == 1)
        fam_count++;
    if (prev_field[i][(j - 1 + W) % W] == 1)
        fam_count++;
    if (prev_field[i][(j + 1 + W) % W] == 1)
        fam_count++;
    if (prev_field[i][j] == 1 && (fam_count == 2 || fam_count == 3)) {
        next_field[i][j] = 1;
        state_checker++;
    } else if (prev_field[i][j] == 0 && fam_count == 3) {
        next_field[i][j] = 1;
        state_checker++;
    } else if (prev_field[i][j] == 1 && (fam_count != 2 || fam_count != 3)) {
        next_field[i][j] = 0;
        state_checker++;
    } else {
        next_field[i][j] = 0;
    }
    return state_checker;
}

void generate(int prev_field[H][W]) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            scanf("%d", &prev_field[i][j]);
        }
    }
}

void set_keypress(void) {
  struct termios new_settings;
  tcgetattr(0, &stored_settings);
  new_settings = stored_settings;
  new_settings.c_lflag &= (~ICANON & ~ECHO);
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}
void reset_keypress(void) {
  tcsetattr(0, TCSANOW, &stored_settings);
  return;
}
