/*
  Multi-threaded socket server
**/
#include <stdio.h>
#include <string.h>    
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MSG_SIZE 100 
#define PATH_SIZE 500 
#define WEBSITE_DIR "/home/alex/Coding/systems-software/file-transfer/server/website"
 
void *connection_handler(void *);
 
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
    puts("Connection accepted");
     
    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = client_sock;
     
    if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0)
    {
      perror("could not create thread");
      return 1;
    }
     
    // Join the thread 
    //pthread_join(sniffer_thread, NULL);
    puts("Handler assigned");
  }
   
  if (client_sock < 0) {
    perror("accept failed");
    return 1;
  }
   
  return 0;
}
 
/*
  Handle client connection
**/
void *connection_handler(void *socket_desc) {
  // Get the socket descriptor
  int sock = *(int*)socket_desc;
  int read_size;
  char *message, client_message[MSG_SIZE], client_filename[MSG_SIZE], 
  client_filepath[MSG_SIZE], full_filepath[PATH_SIZE];
  
  // Receive message from client
  while((read_size = recv(sock, client_message, MSG_SIZE, 0)) > 0) {
    
    // Read client message
    if (strcmp(client_message, "INIT_TRANSFER") == 0) {
      puts("\nStarting transfer");
      
      // Recieve filename
      recv(sock, client_filename, MSG_SIZE, 0); 
      
      // Recieve path
      recv(sock, client_filepath, MSG_SIZE, 0); 
      
      strcpy(full_filepath, WEBSITE_DIR);
      strcat(full_filepath, "/");
      strcat(full_filepath, client_filepath);
      strcat(full_filepath, "/");
      strcat(full_filepath, client_filename);
      puts(full_filepath);
    }

    // Cleanup message memeory
    memset(client_message, 0, sizeof(client_message));
  }
   
  if(read_size == 0) {
    puts("Client disconnected");
    fflush(stdout);
  }
  else if(read_size == -1) {
    perror("recv failed");
  }
       
  // Free socket pointer
  free(socket_desc);
   
  return 0;
}
