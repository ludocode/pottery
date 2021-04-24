BUILD := test/.build/benchmark
RUNNER := $(BUILD)/runner

.PHONY: all
all: $(RUNNER)

CFLAGS := -std=gnu11
CPPFLAGS :=
CXXFLAGS := -std=gnu++17

# We put each benchmark in its own translation unit and don't build with -flto.
# (With LTO, GCC figures out that it can inline qsort() comparison functions.)
CPPFLAGS += -O3 -DNDEBUG
#CPPFLAGS += -Og -DDEBUG -g
CPPFLAGS += -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CPPFLAGS += -Iinclude -Iutil -Iexamples -Itest/src -Itest/.build/lib
CPPFLAGS += -MMD -MP

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
