test: flag_test
	./flag_test

format:
	clang-format -i flag.h flag_test.c example.c

flag_test: flag_test.c flag.h
	cc -o $@ $< -Wall -Wextra -Wpedantic

example: example.c flag.h
	cc -o $@ $< -Wall -Wextra -Wpedantic

.PHONY: test format
