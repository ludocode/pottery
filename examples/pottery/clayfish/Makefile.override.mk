# Include Pottery's example string
C_SRCS += ../string/string.c

# Debug/Release configurations
ifeq ($(DEBUG),1)
# Run "make clean && make DEBUG=1" to build Clayfish in debug mode.
CFLAGS = -Og -g -DDEBUG -fsanitize=address
else
CFLAGS = -O3 -funroll-loops -flto -g
endif

CPPFLAGS += -Wl,-lpthread
