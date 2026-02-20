CC = gcc
CFLAGS = -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -O2 -Iinclude
LFLAGS = -lm

#PREFIX = /usr/local

# cuz i dont want to type every widget after
WIDGETS = $(wildcard src/cli/*.c)
SRCS = src/term.c src/buffers/buf.c src/dom/dom.c src/cli/widgets.c src/gen/gen.c $(WIDGETS)
OBJS = $(SRCS:src/%.c=build/%.o)
LIB  = lib/libfrecli.a

.PHONY: all clean install examples

all: $(LIB) examples

$(LIB): $(OBJS)
	ar rcs $@ $^


build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build
	mkdir -p build/buffers
	mkdir -p build/cli
	mkdir -p build/dom
	mkdir -p build/gen

m: m/muhehe

m/muhehe: m/muhehe.c $(LIB)
	$(CC) $(CFLAGS) $< -L./lib -lfrecli $(LFLAGS) -o $@

clean:
	rm -rf build $(LIB) examples/demo
