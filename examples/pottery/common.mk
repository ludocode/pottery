NAME = $(shell basename "$(shell pwd)")
BUILD = .build/$(NAME)
EXECUTABLE = $(BUILD)/$(NAME)

run: $(EXECUTABLE) FORCE
	@echo "Running $<"
	@echo
	@./$<

build: $(EXECUTABLE) FORCE

clean: FORCE
	rm -rf .build

FORCE:

CPPFLAGS :=
CPPFLAGS += -I../../../include -DPOTTERY_AVAILABLE=1
CPPFLAGS += -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter
ifneq ($(CC),tcc)
CPPFLAGS += -MD -MP
endif

CFLAGS = -O0 -g
LDFLAGS =

C_SRCS := $(shell find . -type f -name '*.c')
CXX_SRCS := $(shell find . -type f -name '*.cxx')

-include Makefile.override.mk

ifeq ($(CXX_SRCS),)
LINK := $(CC)
else
LINK := $(CXX)
endif

C_OBJS := $(patsubst %, $(BUILD)/%.o, $(C_SRCS))
CXX_OBJS := $(patsubst %, $(BUILD)/%.o, $(CXX_SRCS))
ALL_OBJS := $(C_OBJS) $(CXX_OBJS)

$(EXECUTABLE): $(ALL_OBJS)
	$(LINK) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

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
