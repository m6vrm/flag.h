flag.h
======

Command-line flag parsing in C.

>   Inspired by Go flag package: https://pkg.go.dev/flag.

Usage
-----

Add `flag.h` to your project.

```c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define FLAG_IMPLEMENTATION
#include "flag.h"

int main(int argc, char** argv) {
    const char** str_flag = flag_str("str", "Hello, World!", "string flag");
    long long* long_flag = flag_long("long", 42, "long flag");
    bool* bool_flag = flag_bool("bool", true, "bool flag");
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
    printf("long: %lld\n", *long_flag);
    printf("bool: %s\n", *bool_flag ? "true" : "false");

    return EXIT_SUCCESS;
}
```

```
usage: ./example [flags]

  --str=string               string flag (default "Hello, World!")
  --long=number              long flag (default 42)
  --bool                     bool flag (default)
  --help                     show help
```
