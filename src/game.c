#define DS_NO_STDLIB
#define DS_DA_INIT_CAPACITY 16
#define DS_LIST_ALLOCATOR_IMPLEMENTATION
#define DS_DA_IMPLEMENTATION
#define DS_NO_TERMINAL_COLORS
#include "ds.h"
#include "wasm.h"
#include "connect4.h"

DS_ALLOCATOR allocator;
connect4_board_t board;
char player = CONNECT4_PLAYER1;

void print_board(connect4_board_t board) {
    int width = js_width();
    int height = js_height();

    int cell_width = width / CONNECT4_WIDTH;
    int cell_height = height / CONNECT4_HEIGHT;

    js_clear_canvas();
    for (unsigned int col = 0; col < CONNECT4_WIDTH; col++) {
        for (unsigned int row = 0; row < CONNECT4_HEIGHT; row++) {
            int x = col * cell_width;
            int y = row * cell_height;

            js_draw_rect(x, y, cell_width, cell_height, 0xA0A0A0);
            if (board[col][row] == CONNECT4_PLAYER1) {
                js_draw_circle(x + cell_width / 2, y + cell_height / 2, cell_width / 2, 0xFF0000);
            } else if (board[col][row] == CONNECT4_PLAYER2) {
                js_draw_circle(x + cell_width / 2, y + cell_height / 2, cell_width / 2, 0x0000FF);
            } else {
                js_draw_circle(x + cell_width / 2, y + cell_height / 2, cell_width / 2, 0x181818);
            }
        }
    }
}

char swap_player(char player) {
    return (player == CONNECT4_PLAYER1) ? CONNECT4_PLAYER2 : CONNECT4_PLAYER1;
}

void init(void *memory, unsigned long size) {
    DS_INIT_ALLOCATOR(&allocator, memory, size);

    print_board(board);
}

void tick(float deltaTime) {
    int result = 0;
    int move = -1;

    if (player == CONNECT4_PLAYER1) {
        int x = js_canvas_clicked_x();
        if (x == -1) return;

        int cell_width = js_width() / CONNECT4_WIDTH;
        move = x / cell_width;
    } else {
        move = connect4_move(board, player);
    }

    if (connect4_update_board(&board, move, player) == -1) {
        js_log_message("Invalid move for player.", 24);
        result = swap_player(player);
        goto result;
    }

    print_board(board);
    result = connect4_check_win_or_draw(board);
    if (result != 0) {
        goto result;
    }

    player = swap_player(player);
    return;

result:
    if (result == CONNECT4_PLAYER1) {
        js_log_message("Player 1 wins!", 14);
    } else if (result == CONNECT4_PLAYER2) {
        js_log_message("Player 2 wins!", 14);
    } else if (result == CONNECT4_DRAW) {
        js_log_message("It's a draw!", 12);
    }

    for (int i = 0; i < CONNECT4_WIDTH; i++) {
        for (int j = 0; j < CONNECT4_HEIGHT; j++) {
            board[i][j] = 0;
        }
    }
    player = CONNECT4_PLAYER1;
    print_board(board);
}
