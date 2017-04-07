#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
  int SID;
  struct sockaddr_in server;
  char clientMessage[500];
  char serverMessage[500];

  SID = socket(AF_INET, SOCK_STREAM, 0);
  if (SID == -1) {
    printf("Error creating socket\n");
  } else {
    printf("Socket created\n");
  }

  server.sin_port = htons(8081);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;

  if(connect(SID, (struct sockaddr*)&server, sizeof(server)) < 0) {
    printf("Connect failed. Error\n");
    return 1;
  }

  printf("Connected to server\n");

  while(1) {
    printf("Enter a message: ");
    scanf("%s", clientMessage);

    if (send(SID, clientMessage, strlen(clientMessage), 0) < 0) {
      printf("Send failed\n");
      return 1;
    }

    if (recv(SID, serverMessage, 500, 0) < 0) {
      printf("IO error\n");
      break;
    }

    printf("Server sent: ");
    printf("%s", serverMessage);

    close(SID);
  }

  return 0;
}
