LOG_FILE ?=
TARGET = chip8
ROM ?= "./test/roms/programs/Random\ Number\ Test\ \[Matthew\ Mikolay,\ 2010\].ch8"
LOG_LEVEL ?= 1
CC = gcc
CFLAGS = -Wextra -fsanitize=undefined,address -g -c
EXTRA_FLAGS ?= -Werror -Wall 

all: build run

build: compile link

test: clean build_test run_test

build_test: compile_test link_test

compile:
	mkdir -p obj/
	mkdir -p bin/
	mkdir -p log/
	for file in $(wildcard src/*.c); do \
		filename=$$(basename $$file); \
		$(CC) $(CFLAGS) -o obj/$(notdir $${filename%.*}.o) $$file -DLOG_LEVEL=$(LOG_LEVEL); \
	done

compile_test:
	mkdir -p obj/test/
	mkdir -p bin/test/
	mkdir -p log/test/
	for file in $(wildcard src/*.c); do \
		filename=$$(basename $$file); \
		if [ "$$filename" != "main.c" ]; then  \
			$(CC) $(CFLAGS) -o obj/test/$(notdir $${filename%.*}.o) $$file -DLOG_LEVEL=$(LOG_LEVEL); \
		fi \
	done

	for file in $(wildcard test/tests/*.c); do \
		filename=$$(basename $$file); \
		$(CC) $(CFLAGS) -o obj/test/$(notdir $${filename%.*}.o) $$file -DLOG_LEVEL=$(LOG_LEVEL); \
	done

link:
	$(CC) $(CFLAGS) -o bin/$(TARGET) $(wildcard obj/*.o) -DLOG_LEVEL=$(LOG_LEVEL)

link_test:
	$(CC) $(CFLAGS) -o bin/test/$(TARGET) $(wildcard obj/test/*.o) -DLOG_LEVEL=$(LOG_LEVEL)

run:
	./bin/$(TARGET) "$(ROM)"

run_test:
	./bin/test/$(TARGET) "$(ROM)"

.PHONY: debug
debug:
	make build

	if [ ! -f debug/setup.lldb ]; then \
		mkdir -p debug/; \
		touch debug/setup.lldb; \
	fi

	lldb ./bin/$(TARGET) "$(ROM)"

debug_test:
	make build_test

	if [ ! -f debug/test/setup.lldb ]; then \
		mkdir -p debug/test/; \
		touch debug/test/setup.lldb; \
	fi

	lldb -S debug/test/setup.lldb -- ./bin/test/$(TARGET) "$(ROM)"


clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf log/*
