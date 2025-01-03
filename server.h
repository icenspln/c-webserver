#include <sys/socket.h>
#include <netinet/in.h>

struct Server {
    int domain;
    int service; // type
    int protocol;
    __u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;
    void (*launch)(struct Server *server);
};

struct Server server_constructor(int domain, int service, int protocol, __u_long interface, int port, int backlog, void (*launch)(struct Server *server));
