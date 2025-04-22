#include <stdio.h>
#include "connect4.h"

int connect4_move(const connect4_board_t board, char player) {
    (void)(player);

    while (1) {
        int column = 0;
        if (scanf("%d", &column) < 0) return -1;
        int index = column - 1;
        if (!(index < 0 || index >= CONNECT4_WIDTH) && connect4_is_column_full(board, index) == 0) {
            return index;
        }

        return -1;
    }

    return -1;
}
