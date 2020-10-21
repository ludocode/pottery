BUILD := test/build/afl

ifneq (, $(shell which afl-gcc 2>/dev/null))
	CC := afl-gcc
else
	CC := afl-clang
endif

CFLAGS :=
CPPFLAGS :=
CXXFLAGS :=

# we fuzz debug builds by default. you can fuzz release builds with DEBUG=0
ifneq ($(DEBUG), 0)
CPPFLAGS += -DDEBUG -g
endif

# afl wrapper automatically adds -O2
CPPFLAGS += -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CPPFLAGS += -DTEST_POTTERY_FUZZ_MAIN
CPPFLAGS += -Iinclude -Itest/src
CPPFLAGS += -MMD -MP

SRC=test/src/pottery/fuzz
SRCS := $(shell find $(SRC) -type f -name '*.c')
EXES := $(patsubst $(SRC)/%.c, $(BUILD)/%, $(SRCS))

.PHONY: print
print:
	@echo
	@echo "Pass a fuzzer as an argument to test/fuzz.sh:"
	@echo
	@echo "    $(shell echo $(EXES)|sed 's/ /\r    /')"
	@echo
	@echo "You'll probably want to do these first:"
	@echo
	@echo "    echo core | sudo tee /proc/sys/kernel/core_pattern"
	@echo "    echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor"
	@echo '    (undo this last one with "ondemand" instead of "performance")'

.PHONY: all
all: $(EXES)

$(EXES): $(BUILD)/%: $(SRC)/%.c $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	AFL_HARDEN=1 $(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<
	@echo
	@echo "Built $@"
	@echo

# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#depdelete
DEPS := $(EXES:%=%.d)
$(DEPS):
-include $(DEPS)
