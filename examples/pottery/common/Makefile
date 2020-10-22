NAME = $(shell basename "$(shell pwd)")
BUILD = build
EXECUTABLE = $(BUILD)/$(NAME)

run: $(EXECUTABLE) FORCE
	@echo "Running $<"
	@./$<

clean: FORCE
	rm -rf build

FORCE:

CPPFLAGS :=
CPPFLAGS += -I../../../include -DPOTTERY_AVAILABLE=1
CPPFLAGS += -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wfatal-errors
CPPFLAGS += -MD -MP

CFLAGS = -O0 -g

C_SRCS := $(shell find . -type f -name '*.c')
CXX_SRCS := $(shell find . -type f -name '*.cxx')
C_OBJS := $(patsubst %, $(BUILD)/%.o, $(C_SRCS))
CXX_OBJS := $(patsubst %, $(BUILD)/%.o, $(CXX_SRCS))
ALL_OBJS := $(C_OBJS) $(CXX_OBJS)

ifeq ($(CXX_SRCS),)
LINK := $(CC)
else
LINK := $(CXX)
endif

$(EXECUTABLE): $(ALL_OBJS)
	$(LINK) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) -o $@ $^

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
