#include <stdlib.h>
#include "connect4.h"

int connect4_move(const connect4_board_t board, char player) {
    (void)(player);

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
