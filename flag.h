// https://github.com/m6vrm/flag.h
#ifndef FLAG_H
#define FLAG_H

#include <stdbool.h>
#include <stdio.h>

const char** flag_string(const char* name, const char* fallback, const char* summary);
double* flag_number(const char* name, double fallback, const char* summary);
bool* flag_bool(const char* name, bool fallback, const char* summary);
bool flag_parse(int argc, char** argv);
void flag_usage(FILE* out);

#endif  //  FLAG_H

#ifdef FLAG_IMPLEMENTATION

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FLAG_STRING,
    FLAG_NUMBER,
    FLAG_BOOL,
} FlagType;

typedef union {
    const char* as_string;
    double as_number;
    bool as_bool;
} FlagValue;

typedef struct {
    FlagType type;
    const char* name;
    const char* summary;
    FlagValue fallback;
    FlagValue value;
} Flag;

#ifndef FLAG_MAX
#define FLAG_MAX 128
#endif  // FLAG_MAX
static Flag flags[FLAG_MAX];
static int flags_count = 0;
static const char* flag_program;

static Flag* flag_find(const char* name) {
    for (int i = 0; i < flags_count; i++) {
        if (strcmp(flags[i].name, name) == 0)
            return &flags[i];
    }

    return NULL;
}

static Flag* flag_new(FlagType type, const char* name, const char* summary, FlagValue fallback) {
    assert(flags_count < FLAG_MAX);
    assert(flag_find(name) == NULL);
    assert(flag_program == NULL);
    Flag* flag = &flags[flags_count++];
    flag->type = type;
    flag->name = name;
    flag->summary = summary;
    flag->fallback = fallback;
    flag->value = fallback;
    return flag;
}

const char** flag_string(const char* name, const char* fallback, const char* summary) {
    FlagValue value;
    value.as_string = fallback;
    return &flag_new(FLAG_STRING, name, summary, value)->value.as_string;
}

double* flag_number(const char* name, double fallback, const char* summary) {
    FlagValue value;
    value.as_number = fallback;
    return &flag_new(FLAG_NUMBER, name, summary, value)->value.as_number;
}

bool* flag_bool(const char* name, bool fallback, const char* summary) {
    FlagValue value;
    value.as_bool = fallback;
    return &flag_new(FLAG_BOOL, name, summary, value)->value.as_bool;
}

bool flag_parse(int argc, char** argv) {
    assert(argc > 0);
    flag_program = argv[0];
    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        if (strncmp(arg, "--", 2) != 0) {
            fprintf(stderr, "%s: invalid argument\n", arg);
            return false;
        }

        arg += 2;  // skip dashes
        char* name = arg;
        char* value = NULL;
        char* equal = strchr(arg, '=');
        if (equal != NULL) {
            *equal = '\0';
            value = equal + 1;
        }

        Flag* flag = flag_find(name);
        if (flag == NULL) {
            fprintf(stderr, "--%s: unknown flag\n", name);
            return false;
        }

        switch (flag->type) {
            case FLAG_STRING: {
                if (value == NULL) {
                    fprintf(stderr, "--%s: no value provided\n", name);
                    return false;
                }

                flag->value.as_string = value;
            } break;
            case FLAG_NUMBER: {
                if (value == NULL) {
                    fprintf(stderr, "--%s: no value provided\n", name);
                    return false;
                }

                char* end;
                double number = strtod(value, &end);
                if (*end != '\0') {
                    fprintf(stderr, "--%s: invalid number\n", name);
                    return false;
                }

                flag->value.as_number = number;
            } break;
            case FLAG_BOOL: {
                if (value == NULL) {
                    flag->value.as_bool = true;
                } else if (strcmp(value, "true") == 0 || strcmp(value, "1") == 0) {
                    flag->value.as_bool = true;
                } else if (strcmp(value, "false") == 0 || strcmp(value, "0") == 0) {
                    flag->value.as_bool = false;
                } else {
                    fprintf(stderr, "--%s: invalid value\n", name);
                    return false;
                }
            } break;
        }
    }

    return true;
}

void flag_usage(FILE* out) {
    assert(flag_program != NULL);
    fprintf(out, "usage: %s [flags]\n", flag_program);
    if (flags_count > 0)
        fprintf(out, "\n");

    for (int i = 0; i < flags_count; ++i) {
        Flag* flag = &flags[i];

        // name
        char name[64];
        int name_len = 0;
        switch (flag->type) {
            case FLAG_STRING: {
                name_len = snprintf(name, sizeof(name), "  --%s=string", flag->name);
            } break;
            case FLAG_NUMBER: {
                name_len = snprintf(name, sizeof(name), "  --%s=number", flag->name);
            } break;
            case FLAG_BOOL: {
                name_len = snprintf(name, sizeof(name), "  --%s", flag->name);
            } break;
        }

        // summary
        if (name_len <= 27) {
            fprintf(out, "%-27s  %s", name, flag->summary);
        } else {
            fprintf(out, "%s\n%38s", name, flag->summary);
        }

        // default
        switch (flag->type) {
            case FLAG_STRING: {
                if (flag->fallback.as_string != NULL)
                    fprintf(out, " (default \"%s\")", flag->fallback.as_string);
            } break;
            case FLAG_NUMBER: {
                fprintf(out, " (default %.2f)", flag->fallback.as_number);
            } break;
            case FLAG_BOOL: {
                if (flag->fallback.as_bool)
                    fprintf(out, " (default)");
            } break;
        }

        fprintf(out, "\n");
    }
}

#endif  // FLAG_IMPLEMENTATION
