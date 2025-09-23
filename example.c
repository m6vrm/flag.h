#include <stdbool.h>
#include <stdio.h>

#define FLAG_IMPLEMENTATION
#include "flag.h"

int main(int argc, char** argv) {
    const char** string_flag = flag_string("string", "Hello, World!", "string flag");
    double* number_flag = flag_number("number", 42, "number flag");
    bool* bool_flag = flag_bool("bool", true, "bool flag");
    bool* help_flag = flag_bool("help", false, "show help");

    if (!flag_parse(argc, argv)) {
        flag_usage(stderr);
        return 1;
    }

    if (*help_flag) {
        flag_usage(stdout);
        return 1;
    }

    printf("string: %s\n", *string_flag);
    printf("number: %g\n", *number_flag);
    printf("bool: %s\n", *bool_flag ? "true" : "false");

    return 0;
}
