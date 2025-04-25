#include "connect4.h"

static char connect4_check_winner(const connect4_board_t board) {
    // check horizontal
    for (int i = 0; i < CONNECT4_HEIGHT; i++) {
        for (int j = 0; j < CONNECT4_WIDTH - 3; j++) {
            if (board[j][i] != 0 && board[j][i] == board[j + 1][i] &&
                board[j][i] == board[j + 2][i] && board[j][i] == board[j + 3][i]) {
                return board[j][i];
            }
        }
    }

    // check vertical
    for (int i = 0; i < CONNECT4_HEIGHT - 3; i++) {
        for (int j = 0; j < CONNECT4_WIDTH; j++) {
            if (board[j][i] != 0 && board[j][i] == board[j][i + 1] &&
                board[j][i] == board[j][i + 2] && board[j][i] == board[j][i + 3]) {
                return board[j][i];
            }
        }
    }

    // check diagonal
    for (int i = 0; i < CONNECT4_HEIGHT - 3; i++) {
        for (int j = 0; j < CONNECT4_WIDTH - 3; j++) {
            if (board[j][i] != 0 && board[j][i] == board[j + 1][i + 1] &&
                board[j][i] == board[j + 2][i + 2] && board[j][i] == board[j + 3][i + 3]) {
                return board[j][i];
            }
        }
    }
    for (int i = 3; i < CONNECT4_HEIGHT; i++) {
        for (int j = 0; j < CONNECT4_WIDTH - 3; j++) {
            if (board[j][i] != 0 && board[j][i] == board[j + 1][i - 1] &&
                board[j][i] == board[j + 2][i - 2] && board[j][i] == board[j + 3][i - 3]) {
                return board[j][i];
            }
        }
    }

    return 0;
}

char connect4_check_win_or_draw(const connect4_board_t board) {
    char check = connect4_check_winner(board);
    if (check != 0) {
        return check;
    }

    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        if (connect4_is_column_full(board, i) == 0) {
            return 0;
        }
    }

    return CONNECT4_DRAW;
}

int connect4_is_column_full(const connect4_board_t board, int column) {
    for (int i = 0; i < CONNECT4_HEIGHT; i++) {
        if (board[column][i] == 0) {
            return 0;
        }
    }

    return 1;
}

int connect4_update_board(connect4_board_t *board, int column, char player) {
    int result = 0;

    if (column < 0 || column >= CONNECT4_WIDTH) {
        return -1;
    }

    if (connect4_is_column_full(*board, column) == 1) {
        return -1;
    }

    for (int i = CONNECT4_HEIGHT - 1; i >= 0; i--) {
        if ((*board)[column][i] == 0) {
            (*board)[column][i] = player;
            break;
        }
    }

    return result;
}
