#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FLAG_IMPLEMENTATION
#include "flag.h"

extern char* strdup(const char*);

int main(void) {
    const char** str_flag_def = flag_str("str-def", "Hello, World!", "string flag");
    long long* long_flag_def = flag_long("long-def", 42, "long flag");
    bool* bool_flag_def = flag_bool("bool-def", false, "bool flag");

    const char** str_flag = flag_str("str", "Hello, World!", "string flag");
    long long* long_flag = flag_long("long", 42, "long flag");
    bool* bool_flag = flag_bool("bool", false, "bool flag");

    char* args[] = {strdup("flag_test"), strdup("--str=bla bla"), strdup("--long=-69"),
                    strdup("--bool=true")};

    assert(flag_parse(sizeof(args) / sizeof(args[0]), args));
    assert(strcmp(*str_flag_def, "Hello, World!") == 0);
    assert(*long_flag_def == 42);
    assert(*bool_flag_def == false);
    assert(strcmp(*str_flag, "bla bla") == 0);
    assert(*long_flag == -69);
    assert(*bool_flag == true);

    printf("success\n");

    for (size_t i = 0; i < sizeof(args) / sizeof(*args); ++i)
        free(args[i]);

    return 0;
}
