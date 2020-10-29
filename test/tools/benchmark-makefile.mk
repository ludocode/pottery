BUILD := test/build/benchmark
RUNNER := $(BUILD)/runner

.PHONY: all
all: $(RUNNER)

CFLAGS := -std=gnu11
CPPFLAGS :=
CXXFLAGS := -std=gnu++17

# We put each benchmark in its own translation unit and don't build with -flto.
# With LTO, GCC figures out that it can inline the comparison function in our
# qsort() implementation.
CPPFLAGS += -O3
#CPPFLAGS += -Og -DDEBUG -g
CPPFLAGS += -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CPPFLAGS += -Iinclude -Itest/src -Itest/build/lib -Iexamples
CPPFLAGS += -MMD -MP

# An unsafe three-way comparison of ints is sometimes done via "a - b". This
# gives wrong results on large values. A safe comparison instead uses ordinary
# comparison operators.
#
# Ironically, when the comparison function is inlined, the safe comparison is
# marginally faster than the unsafe comparison (like swenson/sort quicksort).
# However the unsafe comparison is faster on algorithms that cannot inline it
# (like qsort() and pottery_qsort().)
#
# This does not affect algorithms that do not need to use three-way comparisons
# (like Pottery templates.)
#
# Change this to see the difference in results.
CPPFLAGS += -DSAFE_THREE_WAY_COMPARE=1

SRC=test/src/pottery/benchmark
C_SRCS := $(shell find $(SRC) -type f -name '*.c') \
	examples/pottery/qsort/pottery_qsort.c \
	examples/pottery/qsort_simple/pottery_qsort_simple.c
CXX_SRCS := $(shell find $(SRC) -type f -name '*.cxx')

C_OBJS := $(patsubst %, $(BUILD)/%.o, $(C_SRCS))
CXX_OBJS := $(patsubst %, $(BUILD)/%.o, $(CXX_SRCS))
ALL_OBJS := $(C_OBJS) $(CXX_OBJS)

$(RUNNER): $(ALL_OBJS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

$(C_OBJS): $(BUILD)/%.o: % $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(CXX_OBJS): $(BUILD)/%.o: % $(MAKEFILE_LIST)
	@mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/#depdelete
DEPS := $(ALL_OBJS:%.o=%.d)
$(DEPS):
-include $(DEPS)
