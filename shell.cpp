#include "shell.h"
#include "io.h"

#include <string.h>
#include <stdlib.h>
#include <string>
#include <exception>

extern "C" {
#include "lisp_balance.h"
#include "lisp_balance.yy.h"
}

extern "C"
int fu_shell_meta(const char * const l) {
    if (!strcmp(l, ".exit")) {
        return 1;
    } else if (!strcmp(l, ".connect ")) {
    } else if (!strcmp(l, ".read ")) {
    } else if (!strcmp(l, ".ping")) {
    }

    return 0;
}

extern "C"
bool fu_judge(const char * const input) {
    bool r;
    if (fu_shell_meta(input)) {
        return true;
    }

    char * const cpy = strdup(input);
    lb_set_input(cpy);

    const int p = lb_lex();
    switch (p) {
        case FULL: {
            try {
                fu_transmit(input);
                fu_response_header_t h;
                const char * const m = fu_recieve(h);
                put_response(h, m);
            } catch (std::exception& e) {
                puts(e.what());
                return NULL;
            }
            r = true;
        } break;
        case PARTIAL: {
            char * b = fu_readline();
            std::string e = std::string() + input + b;
            free(b);
            r = fu_judge(e.c_str());
        } break;
        case BROKEN: {
            puts("Wow budy!");
            r = true;
        } break;
    }

    free(cpy);
    return r;
}

extern "C"
bool fu_interpret() {
    bool r;
    char * input = fu_readline(false);
    r = fu_judge(input);

    free(input);
    return r;
}
