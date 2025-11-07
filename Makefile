CC = gcc

# Compiler flags
CCWARNINGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable
CCOPTS     = -std=c11 -g -O0

# Include and link flags for Check (Homebrew path)
CPPFLAGS += -I/opt/homebrew/opt/check/include
LDFLAGS  += -L/opt/homebrew/opt/check/lib
LDLIBS   = -lcheck -lm

# Combined compiler flags
CFLAGS = $(CCWARNINGS) $(CCOPTS) $(CPPFLAGS)

# Sources and objects
TEST_SOURCES := test_mm.c mm.c memory_setup.c
TEST_OBJECTS := $(TEST_SOURCES:.c=.o)

CHECK_SOURCES := check_mm.c mm.c memory_setup.c
CHECK_OBJECTS := $(CHECK_SOURCES:.c=.o)

APP_SOURCES := main.c io.c mm.c memory_setup.c
APP_OBJECTS := $(APP_SOURCES:.c=.o)

# Executable names
TEST_EXECUTABLE = mm_test
CHECK_EXECUTABLE = malloc_check
APP_EXECUTABLE  = cmd_int

.PHONY: all clean

# Build all targets
all: $(TEST_EXECUTABLE) $(CHECK_EXECUTABLE) $(APP_EXECUTABLE)

# Generic rule for .o files
%.o: %.c mm.h
	$(CC) $(CFLAGS) -c $< -o $@

# Link the test executable
$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

# Link the check executable (unit tests)
$(CHECK_EXECUTABLE): $(CHECK_OBJECTS)
	$(CC) $(CHECK_OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

# Link the main app executable
$(APP_EXECUTABLE): $(APP_OBJECTS)
	$(CC) $(APP_OBJECTS) $(LDFLAGS) -o $@

# Clean rule
clean:
	rm -rf *.o *~ $(TEST_EXECUTABLE) $(CHECK_EXECUTABLE) $(APP_EXECUTABLE)
