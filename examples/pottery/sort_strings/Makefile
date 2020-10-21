EXECUTABLE = build/sort_strings

run: $(EXECUTABLE) FORCE
	@echo "Running $<"
	@./$<

clean: FORCE
	rm -rf build

FORCE:

CPPFLAGS = \
		 -I../../../include -DPOTTERY_AVAILABLE=1 \
		 -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter

CFLAGS = -O0 -g

$(EXECUTABLE): sort_strings.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^
