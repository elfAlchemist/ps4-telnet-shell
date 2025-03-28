// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
#include <arpa/inet.h>

#include "ps4.h"

#define PORT 2323
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    FILE *fp;

    send(client_socket, "Welcome to Telnet Shell!\n", 24, 0);
    while (1) {
        send(client_socket, "> ", 2, 0);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) break;
        buffer[bytes_received] = '\0';
        
        if (strncmp(buffer, "exit", 4) == 0 || strncmp(buffer, "quit", 4) == 0) {
            send(client_socket, "Goodbye!\n", 9, 0);
            break;
        }
        
        // fp = popen(buffer, "r");
        // if (fp) {
        //     while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        //         send(client_socket, buffer, strlen(buffer), 0);
        //     }
        //     pclose(fp);
        // }
    }
    close(client_socket);
}

int _main(struct thread *td) {
  UNUSED(td);

  initKernel();
  initLibc();

  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
      // perror("Socket creation failed");
      return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  
  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      // perror("Bind failed");
      return 1;
  }
  
  if (listen(server_socket, 5) < 0) {
      // perror("Listen failed");
      return 1;
  }
  
  printf("Listening for connections on port %d...\n", PORT);
  
  while (1) {
      client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
      if (client_socket < 0) {
          // perror("Accept failed");
          continue;
      }
      printf("Connected to a client.\n");
      handle_client(client_socket);
  }
  
  close(server_socket);


  return 0;
}
