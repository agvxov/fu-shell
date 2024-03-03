#include "io.h"

#include <string>

#include <readline/readline.h>

inline int fu_fd;
inline const char * prompt1;
inline const char * prompt2 = " > ";

extern "C"
void put_response(fu_response_header_t h, const char * const s) {
    if (!s) {
        puts("");
        return;
    }

    if (!h.error_code) {
        fputs("\033[32m", stdout);
    } else {
        fputs("\033[31m", stdout);
    }
    fputs(s, stdout);

    fputs("\033[0m\n", stdout);
}

extern "C"
void fu_transmit(const char * const s) {
    std::string message = s;
    message = std::string() + (char)0x47 + (char)(message.size() / 256) + (char)(message.size() % 256) + message;
    write(fu_fd, message.c_str(), message.size());
}

extern "C"
const char * const fu_recieve(fu_response_header_t &header) {
    static char buff[MAX_CHUNK];
    static std::string r;

    const int e = read(fu_fd, (void*)(&header), sizeof(fu_response_header_t));
    if (e != sizeof(fu_response_header_t)) {
        return NULL;
    }

    r = "";

    for (int i = 0; i < header.high; i++) {
        const int e2 = read(fu_fd, buff, MAX_CHUNK);
        if (e2 != MAX_CHUNK) {
            return NULL;
        }
        r.append(buff, MAX_CHUNK);
    }

    const int e3 = read(fu_fd, buff, header.low);
    if (e3 != header.low) {
        return NULL;
    }
    r.append(buff, header.low);

    return r.c_str();
}

char * fu_readline(bool is_1) {
    return readline(is_1 ? prompt1 : prompt2);
}
