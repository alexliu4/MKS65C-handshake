#include "pipe_networking.h"

// WKP should be protected by a semaphore

// void byte_reader(char * msg){
//   for (size_t i = 0; i < sizeof(msg); ++i) {
//     // Convert to unsigned char* because a char is 1 byte in size.
//     // That is guaranteed by the standard.
//     // Note that is it NOT required to be 8 bits in size.
//     unsigned char byte = *((unsigned char *)&msg + i);
//     printf("Byte %lu = %u, %c\n", i, (unsigned)byte, (unsigned)byte);
//   }
// }

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/


void server_handshake_part1(int *from_client){
  //printf("Server making WKP\n");
  mkfifo("wkp", 0644);
  //printf("Server made WKP\n");

  //printf("Server opening WKP\n");
  *from_client = open("wkp", O_RDONLY);
  //printf("Server opened WKP\n");
}

int server_handshake_part2(int *to_client, int *from_client) {
  char name[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];

  //printf("Server reading WKP\n");
  read(*from_client, name, sizeof(name));
  printf("%d: Server read \"%s\"\n", getpid(), name);

  //printf("Server opening private pipe\n");
  *to_client = open(name, O_WRONLY);
  //printf("Server opened private pipe\n");

  //printf("Server writing to private pipe\n");
  write(*to_client, ACK, sizeof(ACK));
  printf("%d: Server wrote \"%s\"\n", getpid(), ACK);

  //printf("Server reading from WKP\n";)
  read(*from_client, msg, sizeof(msg));
  printf("%d: Server read \"%s\"\n", getpid(), msg);
  printf("Handshake complete!\n");

  return *from_client;
}
// int server_handshake(int *to_client) {
//   printf("SERVER HANDSHAKE\n" );
//
//   int up = 0;
//   char pid[HANDSHAKE_BUFFER_SIZE];
//   char msg[30];
//   // char pipe1writemessage[30] = "Hello Server!!";
//
//
//   mkfifo("WKP", 0644);
//   printf("WKP pipe created and opened\n");
//   up = open ("WKP", O_RDONLY);
//   //down = open ("server_side", O_RDWR); // kinda useless
//   printf("pipe opened: %d\n", up);
//
//   read(up, pid, sizeof(pid));
//   //semaphores can be used
//   remove("WKP");
//
//   // opens private pipe
//   *to_client = open(pid, O_WRONLY);
//   printf("Server PID given: %d\n", *to_client);
//   // writes message to private pipe for client
//   write(*to_client, ACK, sizeof(ACK));
//
//   // reads client's response from private pipe
//   read(up, msg, sizeof(msg));
//   printf("TEST MESSAGE: %s\n", msg);
//
//   /*==========================UPDATED VERSION==========================*/
//   // reads client's response from pipe
//   while (1){
//     int client_pid = fork();
//     if(client_pid){
//       close(*to_client);
//       //printf("removing wkp\n");
//       remove("wkp");
//       //printf("removed wkp\n");
//     }
//     // clears the msg variable
//     strcpy(msg, "");
//     // byte_reader(msg);
//
//     // read the response and adds str to it
//     read(up, msg, sizeof(msg));
//     printf("FIRST MESSAGE: %s\n", msg);
//     // byte_reader(msg);
//     strcat(msg, "        ~Samuel Konstantinovich 2k18");
//     printf("SECOND MESSAGE: %s\n", msg);
//   }
//
//   return *to_client;
// }


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
// int client_handshake(int *to_server) {
//   printf("CLIENT HANDSHAKE\n" );
//
//   int down = 0;
//   char pid[HANDSHAKE_BUFFER_SIZE];
//   char msg[HANDSHAKE_BUFFER_SIZE];
//   // char pipe1writemessage[30] = "Hello Client!!";
//   sprintf(pid, "%d", getpid());
//
//   mkfifo(pid, 0644);
//   printf("pipe created pid\n");
//   *to_server = open ("WKP", O_WRONLY);
//   //down = open ("server_side", O_RDWR); // kinda useless
//   printf("pipe opened: %d\n", *to_server);
//
//   // Client wrote to WKP
//   write(*to_server, pid, sizeof(pid));
//
//   // Client opens private pipe
//   down = open(pid, O_RDONLY);
//   // reads message from server
//   read(down, msg, sizeof(msg));
//   printf("CLIENT READ SERVER'S MESSAGE '%s'\n", msg);
//
//   // removing private pipe
//   remove(pid);
//
//   // writes message from client to server using WKP
//   write(*to_server, ACK, sizeof(ACK));
//   printf("CLIENT WROTE '%s' TO SERVER\n", ACK);
//
//   /*==========================UPDATED VERSION==========================*/
//   char * line = malloc(100 * sizeof(char));
//
//   while (1){
//     printf("Input a message: ");
//     fgets(line, 1000, stdin);
//
//     write(*to_server, line, sizeof(line));
//     printf("CLIENT WROTE %s", line);
//   }
//
//   free(line);
//
//   return down;
// }
int client_handshake(int *to_server) {
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];

  sprintf(pid, "%d", getpid());

  //printf("Client creating private pipe\n");
  mkfifo(pid, 0644);
  //printf("Client created private pipe\n");

  //printf("Client opening private pipe\n");
  *to_server = open("wkp", O_WRONLY);
  //printf("Client opened private pipe\n");

  //printf("Client writing to WKP\n");
  write(*to_server, pid, sizeof(pid));
  printf("Client wrote\"%s\"\n", pid);

  //printf("Client opening private pipe\n");
  int from_server = open(pid, O_RDONLY);
  //printf("Client opening private pipe\n");

  //printf("Client reading private pipe\n");
  read(from_server, msg, sizeof(msg));
  printf("Client read \"%s\"\n", msg);

  //printf("Client removing private pipe\n");
  remove(pid);
  //printf("Client removed private pipe\n");

  //printf("Client writing to WKP\n");
  write(*to_server, ACK, sizeof(ACK));
  printf("Client wrote \"%s\"\n", ACK);

  printf("Handshake complete!\n==================\n");

  return from_server;
}
