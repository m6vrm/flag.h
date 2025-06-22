flag.h
======

Command-line flag parsing in C.

>   Inspired by Go flag package: https://pkg.go.dev/flag.

Example
-------

```c
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FLAG_IMPLEMENTATION
#include "flag.h"

int main(int argc, char** argv) {
    const char** str_flag = flag_str("str", "Hello, World!", "string flag");
    int64_t* int64_flag = flag_int64("int64", 42, "int64 flag");
    bool* bool_flag = flag_bool("bool", false, "bool flag");
    bool* help_flag = flag_bool("help", false, "show help");

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
```

    cc -o example example.c
    ./example --help

```
usage: ./example [flags]

  --str=string               string flag (default "Hello, World!")
  --int64=number             int64 flag (default 42)
  --bool                     bool flag (default)
  --help                     show help
```
