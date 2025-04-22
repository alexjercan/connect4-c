#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "connect4.h"
#include <dlfcn.h>
#define DS_AP_IMPLEMENTATION
#include "ds.h"

typedef int (*move_t)(const connect4_board_t board, char);

move_t move_player1 = NULL;
move_t move_player2 = NULL;

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

int move_player(connect4_board_t board, char player) {
    if (player == CONNECT4_PLAYER1) {
        return move_player1(board, player);
    } else {
        return move_player2(board, player);
    }
}

typedef struct arguments {
    char *player1;
    char *player2;
} arguments;

void parse_arguments(int argc, char **argv, arguments *args) {
    ds_argparse_parser parser = {0};
    ds_argparse_parser_init(
        &parser,
        "connect4",
        "Connect 4 Game",
        "0.1"
    );

    ds_argparse_add_argument(&parser, (ds_argparse_options){
        .short_name = 'a',
        .long_name = "player1",
        .description = "the strategy for player 1, a path to a DLL",
        .type = ARGUMENT_TYPE_POSITIONAL,
        .required = true,
    });

    ds_argparse_add_argument(&parser, (ds_argparse_options){
        .short_name = 'b',
        .long_name = "player2",
        .description = "the strategy for player 2, a path to a DLL",
        .type = ARGUMENT_TYPE_POSITIONAL,
        .required = true,
    });

    DS_UNREACHABLE(ds_argparse_parse(&parser, argc, argv));

    args->player1 = ds_argparse_get_value(&parser, "player1");
    args->player2 = ds_argparse_get_value(&parser, "player2");

    ds_argparse_parser_free(&parser);
}

int load_move_function(arguments args) {
    void *strat1 = dlopen(args.player1, RTLD_NOW);
    if (strat1 == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }

    move_player1 = (move_t)dlsym(strat1, "connect4_move");
    if (move_player1 == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }

    void *strat2 = dlopen(args.player2, RTLD_NOW);
    if (strat2 == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }

    move_player2 = (move_t)dlsym(strat2, "connect4_move");
    if (move_player2 == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    int move = 0;
    char result = 0;
    char player = CONNECT4_PLAYER1;

    srand(time(NULL));

    arguments args = {0};
    parse_arguments(argc, argv, &args);
    if (load_move_function(args) != 0) {
        fprintf(stderr, "Failed to load move functions.\n");
        return -1;
    }

    connect4_board_t board = {0};
    connect4_display(board);

    while (1) {
        move = move_player(board, player);
        if (move == -1 || connect4_update_board(&board, move, player) == -1) {
            printf("Invalid move for player %d (%d).\n", player, move);
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
