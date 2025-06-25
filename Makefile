.PHONY: test
test: flag_test
	./flag_test

.PHONY: clean
clean:
	$(RM) flag_test
	$(RM) example

.PHONY: format
format:
	clang-format -i flag.h flag_test.c example.c

flag_test: flag_test.c flag.h
	cc -o $@ $< -std=c99 -Wall -Wextra -Wpedantic -g3 -fsanitize=address,undefined

example: example.c flag.h
	cc -o $@ $< -std=c99 -Wall -Wextra -Wpedantic -g3 -fsanitize=address,undefined
