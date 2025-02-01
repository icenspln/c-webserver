#include "server.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void launch(struct Server *);
char *hello();

int main(int argc, char *argv[]) {

  struct Server server =
      server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 7890, 10, launch);
  server.launch(&server);

  return 0;
}

void launch(struct Server *server) {

  char *hello_message = hello();

  int address_len;
  int new_socket;

  while (1) {
    printf("====== LISTENING FOR CONNECTION ======\n");

    address_len = sizeof(struct sockaddr_in);
    new_socket = accept(server->socket, NULL, 0);

    send(new_socket, hello_message, strlen(hello_message), 0);

    close(new_socket);
  }
}
char *hello() {

  FILE *fptr;
  u_long file_size;

  fptr = fopen("index.html", "r");

  if (fptr == NULL) {
    printf("Not able to open the file.");
    exit(1);
  }
  // file_size = sizeof(*fptr);
  fseek(fptr, 0, SEEK_END);
  file_size = ftell(fptr);
  rewind(fptr);

  printf("[DEBUG] opened file size: %lu\n", file_size);

  char *body_buffer = malloc(file_size + 1);
  if (!body_buffer) {
    perror("malloc");
    fclose(fptr);
    return NULL;
  }

  fread(body_buffer, 1, file_size, fptr);

  body_buffer[file_size] = '\0'; // Null-terminate

  printf("[DEBUG] file read into body_buffer: %s\n", body_buffer);
  printf("[DEBUG] body_buffer length %d\n", strlen(body_buffer));

  char *hello_message_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html;"
                                "charset=UTF-8\r\nServer: myServerYaChbab"
                                "(ArchBtw)\r\nDate: ? yes.\r\nConnection:"
                                "keep-alive\r\nCache-Control: no-cache\r\n\r\n";

  char *ret_hello_buffer = malloc(sizeof(*hello_message_headers) + file_size);
  strcpy(ret_hello_buffer, hello_message_headers);
  strcat(ret_hello_buffer, body_buffer);
  printf("[DEBUG] ret_hello_buffer allocated size: %lu\n",
         sizeof(*ret_hello_buffer));
  printf("[DEBUG] ret_hello_buffer : %s\n", ret_hello_buffer);
  return ret_hello_buffer;
}