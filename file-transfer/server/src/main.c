/*
  Pass client accpetted socket
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
  int s;
  int cs;
  int connSize;
  int READSIZE;

  struct sockaddr_in server, client;
  char message[500];

  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    printf("Could not create socket\n");
  } else {
    printf("Socket successfully created\n");
  }

  server.sin_port = htons(8081);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;

  if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Bind issue\n");
    return 1;
  } else {
    printf("Bind complete\n");
  }

  listen(s, 3);
  printf("Waiting for incoming connection from client\n");
  connSize = sizeof(struct sockaddr_in);

  while(1) {
    cs = accept(s, (struct sockaddr *)&client, (socklen_t*)&connSize);
    if (cs < 0) {
      perror("Can't establish connection\n");
      return 1;
    } else {
      printf("Connection from client accepted\n");
    }

    memset(message, 0, 500);
    READSIZE = recv(cs, message, 2000, 0);
    printf("Client said: %s\n", message);
    write(cs, "Recieved", strlen("Recieved"));

    if (READSIZE == 0) {
      puts("Client disconnected\n");
      fflush(stdout);
    } else if(READSIZE == -1) {
      perror("Read error\n");
    }
  }

  return 0;
}
