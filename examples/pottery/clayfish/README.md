# Clayfish

Clayfish is a toy chess engine written in C using Pottery containers and algorithms.

It has a simple command-line interface for playing chess. It also supports the [UCI protocol](https://www.chessprogramming.org/UCI) so it can be used with chess GUIs and chess AI tournament software like [Cute Chess](https://cutechess.com/). Pass `-u` as a command-line argument to run it in UCI mode.

Clayfish demonstrates various uses of Pottery in a "real" application:

- A Pottery [vector](../../../include/pottery/vector/) of moves is used to for move generation, principal variation storage, etc. It uses a large internal capacity to almost always avoid allocations, although it can grow to arbitrary size. This demonstrates how to instantiate a non-static container with a custom `.t.h` file to avoid duplicating the configuration. See [`move.h`](move.h) and [`moves.t.h`](moves.t.h) for the vector configuration.

- Pottery's [quick sort](../../../include/pottery/quick_sort/) is used to sort move lists to improve alpha-beta search. It demonstrates how to use a comparison expression with an external context (the game board.) See it in [`search.c`](search.c).

- Pottery's example [string](../string/) is used extensively in [UCI protocol](https://www.chessprogramming.org/UCI) parsing and general string formatting. Take a look at [`position_format()`](position.c) to see how nice this is compared to standard C strings.

- A Pottery [ring](../../../include/pottery/ring/) of Pottery strings is used to queue input from a background thread that blocks on standard input. This demonstrates how to store a non-bitwise-movable type in a Pottery container. See it in [`uci.c`](uci.c).
