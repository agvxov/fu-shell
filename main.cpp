#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "io.h"
#include "shell.h"

#define PORT 10'008 /* default port */

bool fu_connect(const int port) {
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};

    if ((fu_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return false;
    }

    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(fu_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return false;
    }

    return true;
}

bool init(int argc, char * * argv) {

    prompt1 = getenv("FU_PS1");
    if (not prompt1) {
        prompt1 = PS1;
    }

    const int port =
        (argc > 1)
            ?
        std::stoi(argv[1])
            :
        PORT
    ;

    if (not fu_connect(port)) {
        return false;
    }

    return true;
}

signed main(int argc, char * * argv) {
    if(not init(argc, argv)) {
        return 1;
    }

    while(fu_interpret()) { ; }
    return 0;
}
