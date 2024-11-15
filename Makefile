LOG_FILE ?=
TARGET = chip8
ROM ?=

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
		gcc -Wall -g -c -o obj/$(notdir $${filename%.*}.o) $$file; \
	done

compile_test:
	mkdir -p obj/test/
	mkdir -p bin/test/
	mkdir -p log/test/
	for file in $(wildcard src/*.c); do \
		filename=$$(basename $$file); \
		if [ "$$filename" != "main.c" ]; then  \
			gcc -Wall -g -c -o obj/test/$(notdir $${filename%.*}.o) $$file; \
		fi \
	done

	for file in $(wildcard test/tests/*.c); do \
		filename=$$(basename $$file); \
		gcc -Wall -g -c -o obj/test/$(notdir $${filename%.*}.o) $$file; \
	done

link:
	gcc -Wall -o bin/$(TARGET) $(wildcard obj/*.o)

link_test:
	gcc -Wall -o bin/test/$(TARGET) $(wildcard obj/test/*.o)

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

	lldb -- ./bin/test/$(TARGET) "$(ROM)"


clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf log/*
