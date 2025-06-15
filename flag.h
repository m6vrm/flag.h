// https://github.com/m6vrm/flag.h
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const char** flag_str(const char* name, const char* def, const char* desc);
int64_t* flag_int64(const char* name, int64_t def, const char* desc);
bool* flag_bool(const char* name, bool def, const char* desc);
bool flag_parse(int argc, char** argv);
void flag_usage(FILE* stream);

#ifdef FLAG_IMPLEMENTATION

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FLAG_STR,
    FLAG_INT64,
    FLAG_BOOL,
} FlagType;

typedef union {
    const char* as_str;
    int64_t as_int64;
    bool as_bool;
} FlagValue;

typedef struct {
    FlagType type;
    const char* name;
    const char* desc;
    FlagValue def;
    FlagValue value;
} Flag;

#ifndef FLAG_MAX
#define FLAG_MAX 128
#endif  // FLAG_MAX
static Flag flags[FLAG_MAX];
static int flags_count = 0;
static const char* flag_program;

static Flag* flag_new(FlagType type, const char* name, const char* desc, FlagValue def);
static Flag* flag_find(const char* name);

const char** flag_str(const char* name, const char* def, const char* desc) {
    FlagValue value;
    value.as_str = def;
    return &flag_new(FLAG_STR, name, desc, value)->value.as_str;
}

int64_t* flag_int64(const char* name, int64_t def, const char* desc) {
    FlagValue value;
    value.as_int64 = def;
    return &flag_new(FLAG_INT64, name, desc, value)->value.as_int64;
}

bool* flag_bool(const char* name, bool def, const char* desc) {
    FlagValue value;
    value.as_bool = def;
    return &flag_new(FLAG_BOOL, name, desc, value)->value.as_bool;
}

bool flag_parse(int argc, char** argv) {
    assert(argc > 0);
    flag_program = argv[0];
    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        if (*arg != '-') {
            fprintf(stderr, "%s: invalid argument\n", arg);
            return false;
        }

        ++arg;
        char* name = arg;
        char* value = NULL;
        char* equal = strchr(arg, '=');
        if (equal != NULL) {
            *equal = '\0';
            value = equal + 1;
        }

        Flag* flag = flag_find(name);
        if (flag == NULL) {
            fprintf(stderr, "-%s: unknown flag\n", name);
            return false;
        }

        switch (flag->type) {
            case FLAG_STR: {
                if (value == NULL) {
                    fprintf(stderr, "-%s: no value provided\n", name);
                    return false;
                }

                flag->value.as_str = value;
            } break;
            case FLAG_INT64: {
                if (value == NULL) {
                    fprintf(stderr, "-%s: no value provided\n", name);
                    return false;
                }

                char* end;
                int64_t number = strtoll(value, &end, 10);
                if (*end != '\0') {
                    fprintf(stderr, "-%s: invalid number\n", name);
                    return false;
                }

                flag->value.as_int64 = number;
            } break;
            case FLAG_BOOL: {
                if (value == NULL) {
                    flag->value.as_bool = true;
                } else if (strcmp(value, "true") == 0 || strcmp(value, "1") == 0) {
                    flag->value.as_bool = true;
                } else if (strcmp(value, "false") == 0 || strcmp(value, "0") == 0) {
                    flag->value.as_bool = false;
                } else {
                    fprintf(stderr, "-%s: invalid value\n", name);
                    return false;
                }
            } break;
        }
    }

    return true;
}

void flag_usage(FILE* stream) {
    assert(flag_program != NULL);
    fprintf(stream, "usage: %s [flags]\n", flag_program);
    if (flags_count > 0)
        fprintf(stream, "\nflags:\n");

    for (int i = 0; i < flags_count; ++i) {
        Flag* flag = &flags[i];
        switch (flag->type) {
            case FLAG_STR: {
                fprintf(stream, "  -%s=string\n", flag->name);
                fprintf(stream, "        %s", flag->desc);
                if (flag->def.as_str != NULL)
                    fprintf(stream, " (default \"%s\")\n", flag->def.as_str);
            } break;
            case FLAG_INT64: {
                fprintf(stream, "  -%s=number\n", flag->name);
                fprintf(stream, "        %s", flag->desc);
                fprintf(stream, " (default %" PRIi64 ")\n", flag->def.as_int64);
            } break;
            case FLAG_BOOL: {
                fprintf(stream, "  -%s\n", flag->name);
                fprintf(stream, "        %s\n", flag->desc);
                if (flag->def.as_bool)
                    fprintf(stream, " (default true)\n");
            } break;
        }
    }
}

static Flag* flag_new(FlagType type, const char* name, const char* desc, FlagValue def) {
    assert(flags_count < FLAG_MAX);
    assert(flag_find(name) == NULL);
    assert(flag_program == NULL);
    Flag* flag = &flags[flags_count++];
    flag->type = type;
    flag->name = name;
    flag->desc = desc;
    flag->def = def;
    flag->value = def;
    return flag;
}

static Flag* flag_find(const char* name) {
    for (int i = 0; i < flags_count; i++) {
        if (strcmp(flags[i].name, name) == 0)
            return &flags[i];
    }

    return NULL;
}

#endif  // FLAG_IMPLEMENTATION
