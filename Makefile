LOG_FILE ?= "log.txt"
TARGET = chip8
ROM ?= ./test/roms/programs/Random\ Number\ Test\ \[Matthew\ Mikolay,\ 2010\].ch8
CC = gcc
CFLAGS = -g -c
EXTRA_FLAGS ?= -Werror -Wall -Wextra -fsanitize=undefined,address

PRINT ?= 1

all: build run

build: compile link

test: clean build_test run_test

build_test: compile_test link_test

compile: $(wildcard src/*)
	mkdir -p obj/
	mkdir -p bin/
	mkdir -p log/
	for file in $(wildcard src/*.c); do \
		filename=$$(basename $$file); \
		$(CC) $(CFLAGS) -o obj/$(notdir $${filename%.*}.o) $$file -DLOG_FILE_NAME='$(LOG_FILE)'; \
	done

compile_test: $(wildcard src/*.c) $(wildcard tests/test/*)
	mkdir -p obj/test/
	mkdir -p bin/test/
	mkdir -p log/test/
	for file in $(wildcard src/*.c); do \
		filename=$$(basename $$file); \
		if [ "$$filename" != "main.c" ]; then  \
			$(CC) $(CFLAGS) -o obj/test/$(notdir $${filename%.*}.o) $$file -DLOG_FILE_NAME='"test_log.txt"'; \
		fi \
	done

	for file in $(wildcard test/tests/*.c); do \
		filename=$$(basename $$file); \
		$(CC) $(CFLAGS) -o obj/test/$(notdir $${filename%.*}.o) $$file -DLOG_FILE_NAME='"test_log.txt"'; \
	done

link: compile
	$(CC) -o bin/$(TARGET) $(wildcard obj/*.o)

link_test: compile_test
	$(CC) -o bin/test/$(TARGET) $(wildcard obj/test/*.o)

run: link
	./bin/$(TARGET) $(ROM)

run_test: link_test
	./bin/test/$(TARGET) $(ROM)

.PHONY: debug
debug:
	make build

	if [ ! -f debug/setup.lldb ]; then \
		mkdir -p debug/; \
		touch debug/setup.lldb; \
	fi

	lldb ./bin/$(TARGET) $(ROM)

debug_test:
	make build_test

	if [ ! -f debug/test/setup.lldb ]; then \
		mkdir -p debug/test/; \
		touch debug/test/setup.lldb; \
	fi

	lldb -S debug/test/setup.lldb -- ./bin/test/$(TARGET) $(ROM)

clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf log/*
