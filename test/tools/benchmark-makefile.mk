BUILD := test/build/benchmark
RUNNER := $(BUILD)/runner

.PHONY: all
all: $(RUNNER)

CFLAGS :=
CPPFLAGS := -O3 -flto
#CPPFLAGS := -Og -DDEBUG -g
CXXFLAGS :=

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
# (like qsort().)
#
# This does not affect algorithms that do not need to use three-way comparisons
# (like Pottery.)
#
# Change this to see the difference in results.
CPPFLAGS += -DSAFE_THREE_WAY_COMPARE=1

SRC=test/src/pottery/benchmark
C_SRCS := $(shell find $(SRC) -type f -name '*.c')
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
