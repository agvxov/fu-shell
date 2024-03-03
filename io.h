/* Reading from and writting to the server.
 *  NOTE: the IO has no clue we are using sockets.
 */
#ifndef FU_IO_H
#define FU_IO_H

#include <unistd.h>

/* The script-fu server protocol uses leading bytes
 *  to communicate meta information for clients.
 *  YYY: https://docs.gimp.org/2.10/en/gimp-filters-script-fu.html#plug-in-script-fu-console
 */
struct alignas(1) fu_response_header_t {
    char magic_byte;    // ^G
    char error_code;    // 0 on success
    char high;
    char low;
};

/* Number of bytes fu_response_header::high and fu_response_header::low encode,
 *  as defined by the script-fu documentation.
 */
const int MAX_CHUNK = 256;

/* Socket file descriptor
 */
extern int fu_fd;

#define PS1 "> " /* default value for prompt1 */
extern const char * prompt1;
extern const char * prompt2;

extern "C" {
    void fu_transmit(const char * const s);
    const char * const fu_recieve(fu_response_header_t &header);
    void put_response(fu_response_header_t h, const char * const s);
}

char * fu_readline(bool is_1 = true);

#endif
