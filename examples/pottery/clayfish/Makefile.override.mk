# Debug/Release configurations
ifeq ($(DEBUG),1)
# Run "make clean && make DEBUG=1" to build Clayfish in debug mode.
CFLAGS = -Og -g -DDEBUG -fsanitize=address
else
CFLAGS = -O3 -funroll-loops -flto -g
endif

ifeq ($(CC),tcc)
LDFLAGS += -lpthread
else
LDFLAGS += -pthread
endif
