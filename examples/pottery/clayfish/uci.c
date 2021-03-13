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

#include "uci.h"

#include "position.h"
#include "eval.h"
#include "common.h"
#include "search.h"

#define UCI_LOGFILE "/tmp/clayfish_uci.log"

// A queue of strings used for input. Note that string_t isn't bitwise movable
// so we give its move function to the ring. This also means strings aren't
// passable so insert() and extract() functions will not exist. We have to
// emplace() and remove()/displace() instead. We don't bother passing any other
// string lifecycle functions because we don't care about copying or stealing
// whole queues; we're just using one of them.
#define POTTERY_RING_PREFIX string_queue
#define POTTERY_RING_VALUE_TYPE string_t
#define POTTERY_RING_LIFECYCLE_MOVE string_move
#define POTTERY_RING_LIFECYCLE_DESTROY string_destroy
#include "pottery/ring/pottery_ring_static.t.h"

typedef struct uci_t {
    FILE* log;
    position_t position;
    pthread_t input_thread;

    // go command details
    bool go;
    bool isready;
    int time_ms;
    int increment_ms;
    int movetime_ms;

    // info
    moves_t pv;
    int score;
    int depth;

    // input thread position synchronized under mutex
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    string_queue_t input_queue;
} uci_t;

static void* uci_input_thread(void* vuci);

static void uci_init(uci_t* uci) {
    memset(uci, 0, sizeof(*uci));
    position_set_initial(&uci->position);
    uci->log = fopen(UCI_LOGFILE, "w");
    moves_init(&uci->pv);

    // background input thread
    pthread_mutex_init(&uci->mutex, NULL);
    pthread_cond_init(&uci->condition, NULL);
    string_queue_init(&uci->input_queue);
    pthread_create(&uci->input_thread, NULL, &uci_input_thread, uci);
}



/*
 * Basic input/output and logging
 *
 * Input comes from a background thread that blocks on stdin.
 */

typedef enum uci_channel_t {
    uci_channel_input,
    uci_channel_output,
    uci_channel_error,
    uci_channel_debug,
} uci_channel_t;

// Prints the given string to the given channel.
static void uci_print(uci_t* uci, uci_channel_t channel, const char* str) {
    // We print to each output FILE (log/stdout/stderr) in a single function
    // call to avoid race conditions between threads, this way we don't have to
    // lock anything.

    // Everything goes to the log.
    char prefix;
    switch (channel) {
        case uci_channel_input:  prefix = '<'; break;
        case uci_channel_output: prefix = '>'; break;
        case uci_channel_error:  prefix = 'E'; break;
        case uci_channel_debug:  prefix = 'D'; break;
        default: clayfish_abort();
    }
    fprintf(uci->log, "%c %s\n", prefix, str);
    fflush(uci->log);

    if (channel == uci_channel_output) {
        puts(str);
        fflush(stdout);

    } else if (channel == uci_channel_error) {
        fprintf(stderr, "ERROR: %s\n", str);
        fflush(stderr);

    } else {
        // nothing
    }
}

// Formats and prints the given format string to the given channel.
static void uci_printf(uci_t* uci, uci_channel_t channel, const char* format, ...) {
    char buf[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    buf[sizeof(buf) - 1] = '\0'; // in case of truncation
    uci_print(uci, channel, buf);
}

// Loops blocking on stdin in the background, queuing received lines onto the
// uci input queue.
static void* uci_input_thread(void* vuci) {
    uci_t* uci = (uci_t*)vuci;

    // The input might be huge since it could be a FEN string plus hundreds of
    // moves to apply to it. fgets() isn't really meant for long lines and we
    // don't want to bother implementing something smarter so instead we just
    // allocate a giant buffer for reading a line of input.
    size_t buffer_size = 64*1024;
    char* buffer = (char*)malloc(buffer_size);

    pthread_mutex_lock(&uci->mutex);
    while (true) {
        pthread_mutex_unlock(&uci->mutex);
        char* ret = fgets(buffer, (int)buffer_size, stdin);
        pthread_mutex_lock(&uci->mutex);

        string_t* string = NULL;
        string_queue_emplace_last(&uci->input_queue, &string);
        string_init(string);

        if (ret == NULL) {
            // We failed to read from standard input.

            #if defined(DEBUG)
            // In debug builds we don't exit. This lets us pipe in positions
            // from the commmand-line to test things or fix bugs.
            uci_print(uci, uci_channel_debug, "failed to read from stdin, blocking input");
            pthread_cond_signal(&uci->condition);
            pthread_mutex_unlock(&uci->mutex);
            while(true) {}

            #else
            bool eof = feof(stdin);
            uci_printf(uci, uci_channel_debug, "stdin %s, exiting",
                    eof ? "closed" : "errored (broken pipe?)");
            exit(eof ? EXIT_SUCCESS : EXIT_FAILURE);
            #endif

        } else {
            // fgets() keeps line terminators and the UCI spec says they may
            // not match the platform. (This means we may not be able to use
            // fgets() on Windows. We're not bothering to support Windows right
            // now.) We just strip all whitespace from both ends of the string.
            string_set_cstr(string, ret);
            string_strip_chars_cstr(string, " \t\r\n");
            uci_print(uci, uci_channel_input, string_cstr(string));
        }

        pthread_cond_signal(&uci->condition);
    }
    pthread_mutex_unlock(&uci->mutex);

    // This is unreachable since our loop never breaks: it exits via exit()
    // because there isn't really a sane way to interrupt fgets(). We clean up
    // anyway in case the above loop is changed.
    free(buffer);

    return NULL;
}



/*
 * Input parsing
 */

// A trivial parser that extracts space-delimited words from a string.
typedef struct uci_parser_t {
    const char* pos;
    const char* end;
    string_t word;
} uci_parser_t;

static void uci_parser_init(uci_parser_t* parser, const string_t* input) {
    parser->pos = string_cstr(input);
    parser->end = string_cstr(input) + string_length(input);
    string_init(&parser->word);
}

static void uci_parser_destroy(uci_parser_t* parser) {
    string_destroy(&parser->word);
}

// Pulls a space-delimited word from the parser into the parser's word string.
static bool uci_parser_next_word(uci_parser_t* parser) {

    // spaces before the word should already have been consumed
    if (parser->pos == parser->end || *parser->pos == ' ')
        return false;

    // collect the word
    const char* start = parser->pos;
    while (parser->pos != parser->end && *parser->pos != ' ')
        ++parser->pos;
    string_set_chars(&parser->word, start, (size_t)(parser->pos - start));

    // skip spaces after the word
    while (parser->pos != parser->end && *parser->pos == ' ')
        ++parser->pos;

    return true;
}

// Parses the current word as an int, ignoring errors.
static int uci_parser_int(uci_parser_t* parser) {
    int ret = 0;
    sscanf(string_cstr(&parser->word), "%i", &ret);
    return ret;
}

/**
 * A quick and dirty FEN parser.
 *
 * Contains some validation but it's probably incomplete.
 */
static bool uci_parse_fen(uci_t* uci, uci_parser_t* parser) {
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "FEN string missing!");
        return false;
    }

    // parse board
    {
        const char* str = string_cstr(&parser->word);
        const char* end = str + string_length(&parser->word);

        for (size_t row = 8; row > 0;) {
            --row;

            // parse pieces
            for (size_t col = 0; col < 8; ++col) {
                if (str == end) {
                    uci_print(uci, uci_channel_error, "FEN string truncated parsing pieces!");
                    return false;
                }
                if (*str >= '1' && *str <= '8') {
                    uci->position.board[row][col] = '.';
                    size_t extra = (size_t)(*str - '1');
                    if (col + extra > 8) {
                        uci_print(uci, uci_channel_error, "FEN string has too many blanks in a row!");
                        return false;
                    }
                    for (size_t i = 0; i < extra; ++i)
                        uci->position.board[row][++col] = '.';
                } else {
                    char lower = piece_make_black(*str);
                    if (lower != 'p' && lower != 'n' && lower != 'b' &&
                            lower != 'r' && lower != 'q' && lower != 'k') {
                        uci_printf(uci, uci_channel_debug, "FEN string has invalid piece: '%c'!", *str);
                        return false;
                    }
                    uci->position.board[row][col] = *str;
                }
                ++str;
            }

            // skip / or whitespace
            if (row != 0) {
                if (str == end) {
                    uci_print(uci, uci_channel_error, "FEN string truncated parsing end of row!");
                    return false;
                }
                if (*str != (row == 0 ? ' ' : '/')) {
                    uci_print(uci, uci_channel_error, "FEN string has invalid end of row!");
                    return false;
                }
                ++str;
            }
        }

        if (str != end) {
            uci_print(uci, uci_channel_error, "FEN string board has extra data!");
            return false;
        }
    }

    // player to move
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "FEN string truncated expecting player next to move!");
        return false;
    }
    if (string_equal_char(&parser->word, 'w')) {
        uci->position.white_to_move = true;
    } else if (string_equal_char(&parser->word, 'b')) {
        uci->position.white_to_move = false;
    } else {
        uci_print(uci, uci_channel_error, "FEN string expected \"w\" or \"b\" for player next to move");
        return false;
    }

    // parse castling rights
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "FEN string truncated expecting castling rights!");
        return false;
    }
    if (!string_equal_char(&parser->word, '-')) {
        for (size_t i = 0; i < string_length(&parser->word); ++i) {
            switch (string_cstr(&parser->word)[i]) {
                case 'K': uci->position.castle_white_short = true; break;
                case 'Q': uci->position.castle_white_long = true; break;
                case 'k': uci->position.castle_black_short = true; break;
                case 'q': uci->position.castle_black_long = true; break;
                default:
                    // We don't handle X-FEN Chess960 castling (yet).
                    uci_print(uci, uci_channel_error, "Unrecognized castling rights in FEN string!");
                    return false;
            }
        }
    }

    // parse en-passant
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "FEN string truncated expecting en-passant square!");
        return false;
    }
    if (!string_equal_char(&parser->word, '-')) {
        if (string_length(&parser->word) != 2) {
            uci_print(uci, uci_channel_error, "FEN string en-passant square invalid!");
            return false;
        }
        char col = string_cstr(&parser->word)[0];
        char row = string_cstr(&parser->word)[1];
        if (col < 'a' || col > 'h' || row < '1' || row > '8') {
            uci_print(uci, uci_channel_error, "FEN string en-passant coordinates invalid!");
            return false;
        }
        uci->position.en_passant = coords_make(row - '1', col - 'a');
    }

    // skip half move counter
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "FEN string expected half-move counter!");
        return false;
    }

    // skip full move number
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "FEN string expected full-move number!");
        return false;
    }

    return true;
}

static bool uci_parse_position(uci_t* uci, uci_parser_t* parser) {
    position_set_initial(&uci->position);

    // Parse either "startpos" or "fen <fenstring>"
    if (!uci_parser_next_word(parser)) {
        uci_print(uci, uci_channel_error, "Truncated position command!");
        return false;
    }
    if (string_equal_cstr(&parser->word, "startpos")) {
        uci_printf(uci, uci_channel_debug, "Starting at startpos");
    } else if (string_equal_cstr(&parser->word, "fen")) {
        uci_printf(uci, uci_channel_debug, "Parsing FEN");
        if (!uci_parse_fen(uci, parser))
            return false;
    } else {
        uci_print(uci, uci_channel_error, "Unrecognized position command!");
        return false;
    }

    // We don't require "moves" unless there is a list of moves (the spec isn't
    // clear whether it's optional when there are no moves to follow)
    if (uci_parser_next_word(parser)) {
        if (!string_equal_cstr(&parser->word, "moves")) {
            uci_print(uci, uci_channel_error, "Unrecognized word after initial position!");
            return false;
        }

        // Parse and apply each move
        while (uci_parser_next_word(parser)) {
            move_t move;
            if (!move_parse(&move, &parser->word, uci->position.white_to_move)) {
                uci_print(uci, uci_channel_error, "Error parsing position move!");
                return false;
            }

            char buf[6];
            move_format(&move, buf);
            uci_printf(uci, uci_channel_debug, "Parsed move: %s", buf);

            position_apply_move(&uci->position, &move);
        }
    }

    // Print the final board position to the log
    string_t string;
    string_init(&string);
    position_format(&uci->position, &string);
    uci_printf(uci, uci_channel_debug, "Final position:\n%s", string_cstr(&string));
    string_destroy(&string);

    return true;
}

// Parse a go command.
static bool uci_parse_go(uci_t* uci, uci_parser_t* parser) {
    uci_printf(uci, uci_channel_debug, "got go command, setting running to true");
    uci->go = true;
    uci->time_ms = 0;
    uci->increment_ms = 0;
    uci->movetime_ms = -1;

    while (uci_parser_next_word(parser)) {
        if (string_equal_cstr(&parser->word, "infinite")) {
            uci->time_ms = -1;
            uci->increment_ms = -1;
            uci->movetime_ms = -1;

        } else if (string_equal_cstr(&parser->word, "wtime")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing wtime!");
                return false;
            }
            if (uci->position.white_to_move)
                uci->time_ms = uci_parser_int(parser);

        } else if (string_equal_cstr(&parser->word, "btime")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing btime!");
                return false;
            }
            if (!uci->position.white_to_move)
                uci->time_ms = uci_parser_int(parser);

        } else if (string_equal_cstr(&parser->word, "winc")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing winc!");
                return false;
            }
            if (uci->position.white_to_move)
                uci->increment_ms = uci_parser_int(parser);

        } else if (string_equal_cstr(&parser->word, "binc")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing binc!");
                return false;
            }
            if (!uci->position.white_to_move)
                uci->increment_ms = uci_parser_int(parser);

        } else if (string_equal_cstr(&parser->word, "movetime")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing movetime!");
                return false;
            }
            uci->movetime_ms = uci_parser_int(parser);

        } else if (string_equal_cstr(&parser->word, "movestogo")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing movestogo!");
                return false;
            }
            // It's not clear how much time we'll actually get when we hit
            // movestogo so for now we ignore it.


        // The rest of these are ignored/unsupported

        } else if (string_equal_cstr(&parser->word, "ponder")) {
            uci_printf(uci, uci_channel_debug, "Ignoring go subcommand \"ponder\"");

        } else if (string_equal_cstr(&parser->word, "depth") ||
                string_equal_cstr(&parser->word, "nodes") ||
                string_equal_cstr(&parser->word, "mate")) {
            if (!uci_parser_next_word(parser)) {
                uci_print(uci, uci_channel_error, "go truncated parsing argument for ignored subcommand!");
                return false;
            }
            uci_printf(uci, uci_channel_debug, "Ignoring go subcommand \"%s\"", string_cstr(&parser->word));

        } else if (string_equal_cstr(&parser->word, "searchmoves")) {
            uci_printf(uci, uci_channel_debug, "Ignoring go subcommand \"searchmoves\"");
            // The rest of the string is a move list; return to ignore it
            return true;

        } else {
            // Unrecognized go subcommand. Ignore it and hope it doesn't take
            // any arguments!
            uci_printf(uci, uci_channel_debug, "Ignoring unrecognized go subcommand: \"%s\"", string_cstr(&parser->word));
        }
    }

    return true;
}

void uci_handle_command(uci_t* uci, string_t* input) {

    // Prepare string parser
    uci_parser_t parser;
    uci_parser_init(&parser, input);

    // Blank lines are not technically allowed I don't think, but we ignore
    // them anyway (as we do for unrecognized commands.)
    if (!uci_parser_next_word(&parser))
        return;
    uci_printf(uci, uci_channel_debug, "handling command \"%s\"", string_cstr(&parser.word));

    if (string_equal_cstr(&parser.word, "uci")) {
        uci_print(uci, uci_channel_output, "id name Clayfish");
        uci_print(uci, uci_channel_output, "id author Nicholas Fraser");
        uci_print(uci, uci_channel_output, "uciok");

    } else if (string_equal_cstr(&parser.word, "ucinewgame")) {
        position_set_initial(&uci->position);

    } else if (string_equal_cstr(&parser.word, "isready")) {
        if (uci->go)
            uci->isready = true;
        else
            uci_print(uci, uci_channel_output, "readyok");

    } else if (string_equal_cstr(&parser.word, "position")) {
        // A position command will break if we're still running. We may need to
        // queue up the position for later. For now we just quit.
        if (uci->go) {
            uci_print(uci, uci_channel_error, "Received a \"position\" command while searching!");
            exit(EXIT_FAILURE);
        }
        uci_parse_position(uci, &parser);

        #if 0
        uci_printf(uci, uci_channel_debug, "Starting eval: %i", eval(&uci->position));
        exit(0);
        #endif

    } else if (string_equal_cstr(&parser.word, "go")) {
        uci_parse_go(uci, &parser);

    } else if (string_equal_cstr(&parser.word, "stop")) {
        if (uci->go) {
            uci->go = false;
            uci_printf(uci, uci_channel_debug, "Stopping now.");
        } else {
            uci_printf(uci, uci_channel_debug, "We're already stopped.");
        }

    } else if (string_equal_cstr(&parser.word, "quit")) {
        exit(EXIT_SUCCESS);

    } else {
        // Unrecognized command. Ignore it I guess
    }

    uci_parser_destroy(&parser);
    return;
}

void uci_handle_commands(uci_t* uci) {
    pthread_mutex_lock(&uci->mutex);

    while (!string_queue_is_empty(&uci->input_queue)) {

        // Extract the next input string
        string_t input;
        string_move(&input, string_queue_first(&uci->input_queue));
        string_queue_displace_first(&uci->input_queue);

        // Handle it
        pthread_mutex_unlock(&uci->mutex);
        uci_handle_command(uci, &input);
        string_destroy(&input);
        pthread_mutex_lock(&uci->mutex);
    }

    pthread_mutex_unlock(&uci->mutex);
}



/*
 * Main loop
 */

// We have a main loop here that handles commands when the engine is not
// running. When it gets a command "go", it starts the engine. While the engine
// is running, it periodically calls out to us to determine whether it should
// keep going. This is where we handle commands while running so we can receive
// a "stop" command to stop running.

static bool uci_keep_going(void* context) {
    uci_t* uci = (uci_t*)context;
    uci_handle_commands(uci);
    return uci->go;
}

static void uci_print_info(void* context) {
    uci_t* uci = (uci_t*)context;

    string_t info;
    string_init(&info);
    string_append_format(&info, "info score ");

    // Note that we subtract 1 (rather than add 1) because our scores are
    // actually king captures, not mates.
    if (score_is_mate_by_white(uci->score))
        string_append_format(&info, "mate %i ", (score_mate_by_white - uci->score - 1) / 2);
    else if (score_is_mate_by_black(uci->score))
        string_append_format(&info, "mate -%i ", (uci->score - score_mate_by_black - 1) / 2);
    else
        string_append_format(&info, "cp %i ", uci->score);

    string_append_format(&info, "depth %i pv", uci->depth);

    for (size_t i = 0; i < moves_count(&uci->pv); ++i) {
        string_append_char(&info, ' ');
        char buf[6];
        move_format(moves_at(&uci->pv, i), buf);
        string_append_cstr(&info, buf);
    }

    uci_printf(uci, uci_channel_output, string_cstr(&info));
}

static void uci_loop(uci_t* uci) {
    while (true) {
        uci_handle_commands(uci);

        if (uci->go) {
            uci_printf(uci, uci_channel_debug, "running is true. calling clayfish_go()");

            search(&uci->position,
                    uci->time_ms, uci->increment_ms, uci->movetime_ms,
                    &uci->pv, &uci->score, &uci->depth,
                    &uci_keep_going, &uci_print_info, uci);
            uci_print(uci, uci_channel_debug, "clayfish_go() returned");

            // when go returns, we're done searching
            uci->go = false;

            // print the best move
            move_t move = !moves_is_empty(&uci->pv) ? *moves_first(&uci->pv) : move_make_null();
            char buf[6];
            move_format(&move, buf);
            uci_printf(uci, uci_channel_output, "bestmove %s", buf);

            // print that we're ready if the gui asked isready
            if (uci->isready) {
                uci->isready = false;
                uci_print(uci, uci_channel_output, "readyok");
            }
        } else {
            pthread_mutex_lock(&uci->mutex);
            while (string_queue_is_empty(&uci->input_queue))
                pthread_cond_wait(&uci->condition, &uci->mutex);
            pthread_mutex_unlock(&uci->mutex);
        }
    }
}

int uci_main(void) {
    uci_t uci;
    uci_init(&uci);
    uci_loop(&uci);

    // The above loop function never returns. This is unreachable.
    clayfish_abort();
}
