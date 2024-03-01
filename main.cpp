#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <boost/asio.hpp>

#include "lisp_balance.h"
#include "lisp_balance.yy.h"

using namespace boost::asio;

#define PS1 "> "

io_service hio_service;
ip::tcp::socket hsocket(hio_service);

bool fu_connect() {
    ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), 10008);
    hsocket.connect(endpoint);

    return true;
}

char * transmit(const char * const s) {
    static char reply[1024];
    try {
        std::string message = s;
        message = std::string() + (char)0x47 + (char)(message.size() / 256) + (char)(message.size() % 256) + message;
        boost::system::error_code error;
        write(hsocket, buffer(message), error);
        if (error) {
            throw boost::system::system_error(error);
        }

        size_t reply_length = hsocket.read_some(buffer(reply), error);
        if (error) {
            throw boost::system::system_error(error);
        }
        return reply;
    } catch (std::exception& e) {
        puts(e.what());
        return NULL;
    }
}

int special(const char * const l) {
    if (!strcmp(l, ".exit")) {
        return 1;
    } else if (!strcmp(l, ".connect ")) {
    } else if (!strcmp(l, ".ping")) {
    }

    return 0;
}

void put_response(const char * const s) {
    /* The script-fu server protocol uses leading bytes
     *  to communicate meta information for clients
     *  YYY: https://docs.gimp.org/2.10/en/gimp-filters-script-fu.html#plug-in-script-fu-console
     */
    enum {
        MAGIC_BYTE,
        ERROR_CODE,
        LEN_HIGH,
        LEN_LOW,
        LEADING_META_SIZE,
    };
    const char * const msg = s + LEADING_META_SIZE;

    printf("high: %d low: %d", s[LEN_HIGH], s[LEN_LOW]);

    if (!s[ERROR_CODE]) {
        fputs("\033[32m", stdout);
    } else {
        fputs("\033[31m", stdout);
    }

    for (int i = 0, h = s[LEN_HIGH]; i != h; i++) {
        puts("f");
        fwrite((msg + LEADING_META_SIZE) + (i*256), 256, sizeof(char), stdout);
    }
    fwrite(msg + (s[LEN_HIGH]*256), s[LEN_LOW], sizeof(char), stdout);

    fputs("\033[0m", stdout);
}

bool fu_judge(const char * const input) {
    bool r;
    if (special(input)) {
        return true;
    }

    char * const cpy = strdup(input);
    lb_set_input(input);

    const int p = lb_parse();
    switch (p) {
        default: {
            r = false;
        };
    }

    put_response(transmit(input));
    free(cpy);
    return r;
}

bool fu_interpret() {
    bool r;
    char * input = readline(PS1);
    r = fu_judge();

    free(input);
    return r;
}

signed main() {
    if (not fu_connect()) {
        return 1;
    }

    while(fu_interpret()) { ; }
    return 0;
}
