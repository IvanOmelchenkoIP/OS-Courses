#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	8080
#define MAXLINE 1024

int main() {
  // YOUR CODE HERE
  int socket_descriptor;
  char buffer[MAXLINE];
  struct sockaddr_in server, client;
  memset(&server, 0, sizeof(server));
  memset(&client, 0, sizeof(client));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if ( (socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    printf("Error creating socket...\n");
    return 1;
  }

  if (bind(socket_descriptor, (const struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Error binding to server...\n");
    return 1;
  }

  int client_size = sizeof(client);
  int response_length = recvfrom(socket_descriptor, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *)&client, (socklen_t *)&client_size);
  buffer[response_length] = '\0';
  printf("Client Says: %s\n", buffer);

  char *server_message = "I am fine, thank you.";
  sendto(socket_descriptor, (const char *)server_message, strlen(server_message), MSG_CONFIRM, (const struct sockaddr *) &client, client_size);
  printf("Response sent.\n");
  return 0;
}
