/*
  Socket client
**/
 
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
 
#define MSG_SIZE 100 
#define PATH_SIZE 500 
#define FILE_SIZE 512
#define LOCAL_STORAGE "/home/alex/Coding/systems-software/file-transfer/client/storage/"
 
int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in server;
  char option[2], response[10], message[MSG_SIZE], server_reply[MSG_SIZE], file_dir[PATH_SIZE], username[50], password[50];
   
  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    puts("Could not create socket");
  }
   
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(8888);

  // Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
   
  puts("Connected to server\n");
  
  send(sock, "INIT_LOGIN", strlen("INIT_LOGIN"), 0);  
  
  printf("Username: ");
  scanf("%s", username);
  send(sock, username, strlen(username), 0);
  
  printf("Password: ");
  scanf("%s", password);
  send(sock, password, strlen(password), 0);  
  
  recv(sock, response, 10, 0);
  
  if (strcmp(response, "200") == 0) {
    puts("\nLogin successful\n");
  } else {
    puts("\nLogin unsuccessful\n");
    return 1;
  }
   
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
      printf("> ");
      scanf("%s", message);
      
      strcpy(file_dir, LOCAL_STORAGE);
      strcat(file_dir, message);
      if(access(file_dir, F_OK) == -1 ) {
        puts("File doesn't exist");
        return 1;
      }
      send(sock, message, strlen(message), 0); 
      
      // Send path
      puts("\nChoose a path: ");
      puts("1. Root");
      puts("2. Sales");
      puts("3. Promotions");
      puts("4. Offers");
      puts("5. Marketing");
      printf("> ");
      scanf("%s", option);
      
      if (strcmp(option, "1") == 0) {
        send(sock, "/", strlen("/"), 0); 
      } else if (strcmp(option, "2") == 0) {
        send(sock, "sales", strlen("sales"), 0);
      } else if (strcmp(option, "3") == 0) {
        send(sock, "promotions", strlen("promotions"), 0);
      } else if (strcmp(option, "4") == 0) {
        send(sock, "offers", strlen("offers"), 0);
      } else if (strcmp(option, "5") == 0) {
        send(sock, "marketing", strlen("marketing"), 0);
      } else {
        puts("Not an option");
        return 1;
      }
      
      char file_buffer[FILE_SIZE]; 
      memset(file_buffer, 0, sizeof(file_buffer));
	    int block_size, i = 0; 
	    printf("\nSending %s to server... ", file_dir);
	    FILE *file_open = fopen(file_dir, "r");
	          
	    while((block_size = fread(file_buffer, sizeof(char), FILE_SIZE, file_open)) > 0) {
    		printf("Data sent %d = %d\n", i, block_size);
    		if (send(sock, file_buffer, block_size, 0) < 0) {
            return 1;
        }  
    		memset(file_buffer, 0, sizeof(file_buffer));
    		i++;
      }
      
      recv(sock, response, 10, 0);
      
      if (strcmp(response, "OK") == 0) {
        puts("Transfer successful\n");
      } else {
        puts("Transfer unsuccessful\n");
        close(sock);
        return 1;
      }
      
      fclose(file_open);
    } else if (strcmp(option, "99") == 0) {
      close(sock);
      return 0;
    } else {
      puts("Not valid");
    }
  }
}
