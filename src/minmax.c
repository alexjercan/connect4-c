#include <stdlib.h>
#include "ds.h"
#include "connect4.h"

#ifndef MINMAX_DEPTH
#define MINMAX_DEPTH 5
#endif

#define MINMAX_INF 1000000

typedef struct move_score {
    int move;
    int score;
} move_score;

static move_score minmax(const connect4_board_t board, char player, char maxxing, int depth, int alpha, int beta);
static int eval(const connect4_board_t board, char player);

int connect4_move(const connect4_board_t board, char player) {
    return minmax(board, player, player, MINMAX_DEPTH, -MINMAX_INF, MINMAX_INF).move;
}

static char swap_player(char player) {
    return (player == CONNECT4_PLAYER1) ? CONNECT4_PLAYER2 : CONNECT4_PLAYER1;
}

static int eval(const connect4_board_t board, char player) {
    connect4_board_t weights = {
        {1, 1, 1, 1, 1, 1},
        {1, 3, 3, 3, 3, 1},
        {1, 3, 7, 7, 3, 1},
        {1, 7, 9, 9, 7, 1},
        {1, 3, 7, 7, 3, 1},
        {1, 3, 3, 3, 3, 1},
        {1, 1, 1, 1, 1, 1},
    };

    int score_1 = 0;
    int score_2 = 0;
    for (int j = 0; j < CONNECT4_HEIGHT; j++) {
        for (int k = 0; k < CONNECT4_WIDTH; k++) {
            score_1 += weights[k][j] * (board[k][j] == CONNECT4_PLAYER1);
            score_2 += weights[k][j] * (board[k][j] == CONNECT4_PLAYER2);
        }
    }

    return (player == CONNECT4_PLAYER1) ? score_1 - score_2 : score_2 - score_1;
}

int random_move(const connect4_board_t board) {
    int count = 0;
    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        if (connect4_is_column_full(board, i) == 0) {
            count++;
        }
    }

    if (count == 0) {
        return -1;
    }

    int column = rand() % count;
    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        if (connect4_is_column_full(board, i) == 0) {
            if (column == 0) {
                return i;
            }
            column--;
        }
    }

    return -1;
}

static move_score minmax(const connect4_board_t board, char player, char maxxing, int depth, int alpha, int beta) {
    char result = connect4_check_win_or_draw(board);
    if (result != 0) {
        move_score value = {.score = 0, .move = -1};
        if (result == CONNECT4_DRAW) return value;

        int good = maxxing == result;
        value.score = good ? MINMAX_INF : -MINMAX_INF;
        return value;
    }

    if (depth == 0) {
        move_score value = {.score = 0, .move = -1};
        value.score = eval(board, player);
        return value;
    }

    move_score best = {.score = 0, .move = -1};
    if (maxxing == player) best.score = -MINMAX_INF;
    else best.score = MINMAX_INF;
    best.move = random_move(board);

    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        if (connect4_is_column_full(board, i) == 0) {
            connect4_board_t copy = {0};
            for (int j = 0; j < CONNECT4_HEIGHT; j++) {
                for (int k = 0; k < CONNECT4_WIDTH; k++) {
                    copy[k][j] = board[k][j];
                }
            }
            if (connect4_update_board(&copy, i, player) == -1) {
                DS_PANIC(DS_ERROR_UNREACHABLE);
            }
            move_score value = minmax(copy, swap_player(player), maxxing, depth - 1, alpha, beta);

            if (maxxing == player) {
                if (value.score > best.score) {
                    best.score = value.score;
                    best.move = i;
                }

                if (value.score > alpha) alpha = value.score;
            } else {
                if (value.score < best.score) {
                    best.score = value.score;
                    best.move = i;
                }

                if (value.score < beta) beta = value.score;
            }

            if (alpha > beta) break;
        }
    }

    return best;
}
