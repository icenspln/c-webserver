#include "HTTPRequest.h"
#include "../DataStructures/Lists/Queue.h"
#include <stdlib.h>
#include <string.h>

int return_method(char *method) {
  if (strcmp(method, "GET") == 0) {
    return GET;
  } else if (strcmp(method, "POST") == 0) {
    return POST;
  } else if (strcmp(method, "PUT") == 0) {
    return PUT;
  } else if (strcmp(method, "HEAD") == 0) {
    return HEAD;
  } else if (strcmp(method, "PATCH") == 0) {
    return PATCH;
  } else if (strcmp(method, "DELETE") == 0) {
    return DELETE;
  } else if (strcmp(method, "CONNECT") == 0) {
    return CONNECT;
  } else if (strcmp(method, "OPTIONS") == 0) {
    return OPTIONS;
  } else if (strcmp(method, "TRACE") == 0) {
    return TRACE;
  } else {
    return -1;
  }
}

struct HTTPRequest http_request_constructor(char *request_string) {

  struct HTTPRequest request;
  char request_local[strlen(request_string)];
  strcpy(request_local, request_string);

  for (int i = 0; i < strlen(request_local) - 1; i++) {
    if (request_local[i] == '\n' && request_local[i + 1] == '\n') {
      request_local[i + 1] = '|';
    }
  }
  printf("[DEBUG] request_local:\n%s", request_local);

  char *request_line = strtok(request_local, "\n");
  char *request_header_fields = strtok(NULL, "|");
  char *request_body = strtok(NULL, "|");

  printf("\n[DEBUG] parsed request :\n%s\n%s\n%s", request_line,
         request_header_fields, request_body);

  char *request_method_string = strtok(request_line, " ");
  char *URI = strtok(NULL, " ");
  char *HTTPVersion = strtok(NULL, "\n");
  HTTPVersion = strtok(HTTPVersion, "/");
  HTTPVersion = strtok(NULL, "\n");

  request.Method = return_method(request_method_string);
  request.URI = URI;
  request.HTTPVersion = strtof(HTTPVersion, NULL);

  request.header_fields = dictionary_constructor(compare_string_keys);
  struct Queue headers = queue_constructor();

  char *token = strtok(request_header_fields, "\n");
  while (token) {
    headers.push(&headers, token, sizeof(*token));
    token = strtok(NULL, "\n");
  }

  char *single_header_string = (char *)headers.peek(&headers);
  while (single_header_string) {
    char *key = strtok(single_header_string, ":");
    char *value = strtok(NULL, "|");
    request.header_fields.insert(&request.header_fields, key, sizeof(*key),
                                 value, sizeof(*value));

    headers.pop(&headers);
    single_header_string = (char *)headers.peek(&headers);
  }

  return request;
}