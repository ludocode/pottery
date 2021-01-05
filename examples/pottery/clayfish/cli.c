/*
 * MIT License
 *
 * Copyright (c) 2020-2021 11037650 Canada Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "search.h"
#include "position.h"
#include "generate.h"


/*
 * Command-line UI
 */

static void cli_update_ai_move(void* vpv) {
    moves_t* pv = (moves_t*)vpv;
    if (moves_is_empty(pv))
        return;

    // erase the previous ai move and replace it
    fprintf(stdout, "\b\b\b\b\b\b");
    move_t* move = moves_first(pv);
    move_print(move);
    if (move->promotion == '.')
        putchar(' ');
    putchar(' ');
    fflush(stdout);
}

static void cli_ai_move(position_t* position) {
    printf("Thinking...       "); // extra spaces to update ai move
    fflush(stdout);

    int score, depth;
    moves_t pv;
    moves_init(&pv);

    search(position,
            0, 0, 3000,
            &pv, &score, &depth,
            NULL, cli_update_ai_move, &pv);

    move_t move = !moves_is_empty(&pv) ? *moves_first(&pv) : move_make_null();
    moves_destroy(&pv);

    printf("\nMy move: ");
    move_print(&move);
    printf("  %c%i.%i  depth %i\n\n",
            score > 0 ? '+' : '-', abs(score) / 100, abs(score) % 100,
            depth);

    position_apply_move(position, &move);
}

static void cli_print_board(position_t* position) {
    string_t string;
    string_init(&string);
    position_format(position, &string);
    puts(string_cstr(&string));
    putchar('\n');
    string_destroy(&string);
}

/**
 * Prompts the user for a move.
 *
 * Returns false if the user passed.
 */
static bool cli_prompt_user_move(position_t* position) {
    move_t move;

    while (true) {
        printf("Enter move: ");
        fflush(stdout);

        char buf[64];
        buf[0] = '\0';
        if (NULL == fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput closed. Exiting.");
            exit(ferror(stdin) ? EXIT_FAILURE : EXIT_SUCCESS);
        }

        string_t string;
        string_init_cstr(&string, buf);
        string_strip_chars_cstr(&string, " \t\r\n");

        if (string_equal_cstr(&string, "pass"))
            return false;

        move = move_make_null();
        if (!move_parse(&move, &string, position->white_to_move)) {
            puts("Invalid move!");
            continue;
        }

        if (!position_is_move_legal(position, &move)) {
            puts("Illegal move!");
            continue;
        }

        break;
    }

    putchar('\n');
    position_apply_move(position, &move);
    return true;
}

bool cli_check_game_over(position_t* position, bool ai_turn) {
    position_state_t state = position_state(position);

    switch (state) {
        case position_state_normal:
            return false;

        case position_state_check:
            puts(ai_turn ? "Clayfish is in check." : "You are in check.");
            return false;

        case position_state_checkmate:
            printf("Checkmate. %s wins.\n", position->white_to_move ? "Black" : "White");
            return true;

        case position_state_stalemate:
            puts("Stalemate. The game is a draw.");
            return true;
    }

    abort();
}

int cli_main(void) {
    puts("");
    puts("This is Clayfish!");
    puts("");
    puts("Enter moves in simplified long algebraic notation. e.g.:");
    puts("    d2d4   for a queen's pawn opening");
    puts("    e8g8   for black short castling");
    puts("    g7g8n  to promote to a knight");
    puts("    pass   to let Clayfish move and switch sides");
    puts("");

    position_t position;
    position_set_initial(&position);

    bool ai_turn = false;

    while (true) {
        cli_print_board(&position);

        if (cli_check_game_over(&position, ai_turn))
            break;

        if (!ai_turn) {
            if (cli_prompt_user_move(&position)) {
                ai_turn = true;
                continue;
            }
        }

        ai_turn = false;
        cli_ai_move(&position);
    }

    return EXIT_SUCCESS;
}
