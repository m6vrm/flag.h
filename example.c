#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FLAG_IMPLEMENTATION
#include "flag.h"

int main(int argc, char** argv) {
    const char** str_flag = flag_str("str", "Hello, World!", "String flag");
    int64_t* int64_flag = flag_int64("int64", 42, "Int64 flag");
    bool* bool_flag = flag_bool("bool", false, "Bool flag");
    bool* help_flag = flag_bool("help", false, "Show help");

    if (!flag_parse(argc, argv)) {
        flag_usage(stderr);
        return EXIT_FAILURE;
    }

    if (*help_flag) {
        flag_usage(stdout);
        return EXIT_SUCCESS;
    }

    printf("str: %s\n", *str_flag);
    printf("int64: %" PRIi64 "\n", *int64_flag);
    printf("bool: %s\n", *bool_flag ? "true" : "false");

    return EXIT_SUCCESS;
}
