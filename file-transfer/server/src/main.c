#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "connection_handler.h"

/*
  Multi-threaded socket server
**/
int main(int argc , char *argv[]) {
  int socket_desc, client_sock, c , *new_sock;
  struct sockaddr_in server, client;

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    puts("Could not create socket");
  }
  puts("Socket created");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8888);

  // Bind
  if(bind(socket_desc,(struct sockaddr *) &server, sizeof(server)) < 0) {
    perror("bind failed. Error");
    return 1;
  }
  puts("Bind completed");

  // Listen
  listen(socket_desc , 3);

  // Accept incoming connections
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);
  while((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*)&c))) {
    puts("\nConnection accepted");

    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = client_sock;

    if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0) {
      perror("could not create thread");
      return 1;
    }

    puts("Handler assigned");
  }

  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }

  return 0;
}
