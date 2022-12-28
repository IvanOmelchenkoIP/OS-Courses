#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

int main() {

  // YOUR CODE HERE
  int socket_id;
  char *buffer[MAXLINE];
  struct sockaddr_in client, server;
  memset(&client, 0, sizeof(client));
  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if (socket_id = socket(AF_INET, SOCK_DGRAM, 0) < 0) {
    printf("Error creating socket...");
    return 1;
  }

  if (bind(socket_id, (const struct sockaddr *)&server, sizeof(server)) < 0) {
    printf("Error binding to server...");
    return 1;
  }

  int size = recvfrom(socket_id, (char *)buffer, MAXLINE,
                MSG_WAITALL, (struct sockaddr *) &client, &sizeof(client));
  buffer[size] = '\0';
  printf("Client Says: %s\n", buffer);

 char *server_message = "I am file, thank you."
  sendto(socket_id, (const char *)server_message, strlen(server_message),
        MSG_CONFIRM, (const struct sockaddr *) &client, sizeof(client));
  printf("Response sent.\n");

  return 0;
}

