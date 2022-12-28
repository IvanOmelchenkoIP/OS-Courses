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
	int socket_descriptor;
	char *buffer[MAXLINE];
	struct sockaddr_in client, server;
	memset(&client, 0, sizeof(client));
	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

	if (socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0) < 0) {
		printf("Error creating socket...\n");
		return 1;
	}

	/*if (*/bind(socket_descriptor, (const struct sockaddr *)&server, sizeof(server)); /*< 0) {
    printf("Error binding to server...\n");
		return 1;
  } */

  int size = (int)recvfrom(socket_descriptor, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&client, (socklen_t *)sizeof(client));
  buffer[size] = '\0';
  printf("Client Says: %s\n", *buffer);

	char *server_message = "I am fine, thank you.";
  sendto(socket_descriptor, (const char *)server_message, strlen(server_message),
        MSG_CONFIRM, (const struct sockaddr *) &client, sizeof(client));
  printf("Response sent.\n");

	return 0;
}
