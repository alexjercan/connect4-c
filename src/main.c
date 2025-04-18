#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "connect4.h"

const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";

void print_result(char result) {
    if (result == CONNECT4_PLAYER1) {
        printf("Player 1 wins!\n");
    } else if (result == CONNECT4_PLAYER2) {
        printf("Player 2 wins!\n");
    } else if (result == CONNECT4_DRAW) {
        printf("It's a draw!\n");
    }
}

char swap_player(char player) {
    return (player == CONNECT4_PLAYER1) ? CONNECT4_PLAYER2 : CONNECT4_PLAYER1;
}

void print_board(connect4_board_t board) {
    (void)(system("stty cooked"));
    printf("%s", CLEAR_SCREEN_ANSI);
    connect4_display(board);
    (void)(system("stty raw"));
}

int main(void) {
    int move = 0;
    char result = 0;
    char player = CONNECT4_PLAYER1;

    srand(time(NULL));

    connect4_board_t board = {0};
    connect4_display(board);

    while (1) {
        move = connect4_move(board, player);
        if (move == -1 || connect4_update_board(&board, move, player) == -1) {
            printf("Invalid move for player %d.\n", player);
            result = swap_player(player);
            break;
        }
        print_board(board);
        result = connect4_check_win_or_draw(board);
        if (result != 0) {
            break;
        }
        player = swap_player(player);
    }
    print_result(result);

    return 0;
}
