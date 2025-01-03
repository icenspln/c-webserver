#include "server.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void launch(struct Server *);

int main(int argc, char *argv[]) {

  struct Server server =
      server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 7890, 10, launch);
  server.launch(&server);

  return 0;
}

void launch(struct Server *server) {

  char buffer[30000];
  char *helloMessage =
      "HTTP/1.1 200 OK\r\nContent-Type: text/html; "
      "charset=UTF-8\r\nContent-Length: 68\r\nServer: Apache/2.4.41 "
      "(Ubuntu)\r\nDate: Fri, 21 Dec 2024 12:00:00 GMT\r\nConnection: "
      "keep-alive\r\nCache-Control: no-cache\r\n\r\n<html><body><h1>Hello, "
      "World!</h1></body></html>";

  int address_len;
  int new_socket;

  while (1) {
    printf("====== LISTENING FOR CONNECTION ======\n");

    address_len = sizeof(struct sockaddr_in);
    new_socket = accept(server->socket, NULL, 0);

    read(new_socket, buffer, 30000);
    printf("%s\n", buffer);

    send(new_socket, helloMessage, strlen(helloMessage), 0);

    close(new_socket);
  }
}
