/*
  Socket client
**/
 
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
 
#define MSG_SIZE 100 
 
int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in server;
  char option[2], message[MSG_SIZE], server_reply[MSG_SIZE];
   
  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    puts("Could not create socket");
  }
  //puts("Socket created");
   
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(8888);

  // Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
   
  puts("Connected\n");
   
  // Keep communicating with server
  while(1) {
    puts("Select from the menu:\n");
    puts("  1)  Transfer a file");
    puts("  99) Logout");
    printf("\n> ");
    scanf("%s", option);
    
    if (strcmp(option, "1") == 0) {
      puts("\nStarting transfer");
      
      // Initiate transfer
      send(sock, "INIT_TRANSFER", strlen("INIT_TRANSFER"), 0);  
      
      // Send filename
      puts("\nEnter a filename: ");
      printf("\n> ");
      scanf("%s", message);
      send(sock, message, strlen(message), 0); 
      
      // Send path
      puts("\nChoose a path: ");
      puts("1. Root");
      puts("2. Sales");
      puts("3. Promotions");
      puts("4. Offers");
      puts("5. Marketing");
      printf("\n> ");
      scanf("%s", option);
      
      if (strcmp(option, "1") == 0) {
        send(sock, "/", strlen("/"), 0); 
      } else if (strcmp(option, "2") == 0) {
        send(sock, "/sales", strlen("/sales"), 0);
      } else if (strcmp(option, "3") == 0) {
        send(sock, "/promotions", strlen("/promotions"), 0);
      } else if (strcmp(option, "4") == 0) {
        send(sock, "/offers", strlen("/offers"), 0);
      } else if (strcmp(option, "5") == 0) {
        send(sock, "/marketing", strlen("/marketing"), 0);
      } else {
        puts("Not an option");
        return 0;
      }
      
    } else if (strcmp(option, "99") == 0) {
      close(sock);
      return 0;
    } else {
      puts("Not valid");
    }
  }
}
