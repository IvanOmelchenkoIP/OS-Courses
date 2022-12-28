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
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

	if (socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0) < 0) {
		printf("Error creating socket...\n");
		return 1;
	}

	char *client_message = "Hello, how are you?";
  sendto(socket_descriptor, (const char *)client_message, strlen(client_message), MSG_CONFIRM, (const struct sockaddr *)&server, sizeof(server));
  printf("Hello message sent.\n");

  int size = recvfrom(socket_descriptor, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&server, (socklen_t *)sizeof(server));
  buffer[size] = '\0';
  printf("Client Says: %s\n", *buffer);

	return 0;
}
