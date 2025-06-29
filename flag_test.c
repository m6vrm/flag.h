#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define FLAG_IMPLEMENTATION
#include "flag.h"

extern char* strdup(const char*);

int main(void) {
    const char** string_flag_def = flag_string("string-def", "Hello, World!", "string flag");
    double* number_flag_def = flag_number("number-def", 42, "number flag");
    bool* bool_flag_def = flag_bool("bool-def", false, "bool flag");

    const char** string_flag = flag_string("string", "Hello, World!", "string flag");
    double* number_flag = flag_number("number", 42, "number flag");
    bool* bool_flag = flag_bool("bool", false, "bool flag");

    char* args[] = {strdup("flag_test"), strdup("--string=bla bla"), strdup("--number=-69"),
                    strdup("--bool=true")};

    assert(flag_parse(sizeof(args) / sizeof(args[0]), args));

    assert(strcmp(*string_flag_def, "Hello, World!") == 0);
    assert(*number_flag_def == 42);
    assert(*bool_flag_def == false);

    assert(strcmp(*string_flag, "bla bla") == 0);
    assert(*number_flag == -69);
    assert(*bool_flag == true);

    printf("success\n");

    for (size_t i = 0; i < sizeof(args) / sizeof(*args); ++i)
        free(args[i]);

    return 0;
}
