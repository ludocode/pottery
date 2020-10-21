EXECUTABLE = build/int_vector

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

$(EXECUTABLE): int_vector.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^
