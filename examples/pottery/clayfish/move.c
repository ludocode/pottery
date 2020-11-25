#include "move.h"

#include "moves.t.h"
#include "pottery/vector/pottery_vector_define.t.h"

void move_format(move_t* move, char buf[6]) {
    if (coords_is_null(move->from)) {
        strcpy(buf, "0000");
        return;
    }

    buf[0] = (char)(move->from.col + 'a'),
    buf[1] = (char)(move->from.row + '1'),
    buf[2] = (char)(move->to.col + 'a'),
    buf[3] = (char)(move->to.row + '1');
    if (move->promotion != '.') {
        buf[4] = piece_make_black(move->promotion);
        buf[5] = '\0';
    } else {
        buf[4] = '\0';
    }
}

void move_print(move_t* move) {
    char buf[6];
    move_format(move, buf);
    fputs(buf, stdout);
}

static inline bool move_parse_coord(char input, char base, signed char* out) {
    if (input < base || input > base + 7)
        return false;
    *out = (signed char)(input - base);
    return true;
}

bool move_parse(move_t* move, string_t* string, bool white_to_move) {
    const char* cstr = string_cstr(string);
    size_t len = string_length(string);
    if (len != 4 && len != 5)
        return false;

    if (!move_parse_coord(cstr[0], 'a', &move->from.col) ||
            !move_parse_coord(cstr[1], '1', &move->from.row) ||
            !move_parse_coord(cstr[2], 'a', &move->to.col) ||
            !move_parse_coord(cstr[3], '1', &move->to.row))
        return false;

    if (len == 5) {
        piece_t promotion = piece_make_black(cstr[4]);
        if (promotion != 'q' && promotion != 'r' &&
                promotion != 'b' && promotion != 'n')
            return false;
        move->promotion = white_to_move ? piece_make_white(promotion) : promotion;
    } else {
        move->promotion = '.';
    }

    return true;
}
