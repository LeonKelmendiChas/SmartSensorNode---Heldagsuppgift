CC = gcc
OPTIMIZE = -ffunction-sections -fdata-sections -O2 -flto -Wno-unused-result -fno-strict-aliasing
DEBUG_FLAGS = -g -O0 -Wfatal-errors -Werror
LIBS =
INCLUDES =
MODE ?= release
CFLAGS_BASE = -Wall -Wno-psabi -Wfatal-errors -Werror

ifeq ($(MODE),debug)
  CFLAGS = $(CFLAGS_BASE) $(DEBUG_FLAGS)
  LDFLAGS =
else
  CFLAGS = $(CFLAGS_BASE) $(OPTIMIZE)
  LDFLAGS =
endif

SRC_DIR = src
BUILD_DIR = build
SOURCES := $(shell find -L $(SRC_DIR) -type f -name '*.c')
CLIENT_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/client/%.o,$(SOURCES))
EXECUTABLES = client

all: $(EXECUTABLES)
	@echo "Build complete ($(MODE))."

debug-client:
	@$(MAKE) MODE=debug --no-print-directory clean client
	@-rm -f WADEBUG.txt
	gdb client -ex run

client: $(CLIENT_OBJECTS)
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/client/%.o: $(SRC_DIR)/%.c
	@echo "Compiling (client) $<..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -DTCPCLIENT -c $< -o $@

FILE =
compile:
	@echo "Compiling $(FILE) (client defs)..."
	@mkdir -p $(BUILD_DIR)/client/$(dir $(FILE))
	$(CC) $(CFLAGS) $(INCLUDES) -DTCPCLIENT -c $(FILE) -o $(BUILD_DIR)/client/$(FILE:.c=.o)

clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR) $(EXECUTABLES)

.PHONY: all clean compile debug-client
