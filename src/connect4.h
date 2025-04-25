#ifndef CONNECT4_H
#define CONNECT4_H

#define CONNECT4_WIDTH 7
#define CONNECT4_HEIGHT 6

#define CONNECT4_PLAYER1 1
#define CONNECT4_PLAYER2 2
#define CONNECT4_DRAW 3

typedef char connect4_board_t[CONNECT4_WIDTH][CONNECT4_HEIGHT];

char connect4_check_win_or_draw(const connect4_board_t board);
int connect4_is_column_full(const connect4_board_t board, int column);
int connect4_update_board(connect4_board_t *board, int column, char player);

// This function is used to get the next move for the player.
// This can return -1 if there is no valid move or the AI sucks.
int connect4_move(const connect4_board_t board, char player);

#endif // CONNECT4_H
