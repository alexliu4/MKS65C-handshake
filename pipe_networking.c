#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int up;
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];
  char pipe1writemessage[30] = "Hello Server!!";


  mkfifo("WKP", 0644);
  printf("pipe created WKP\n");
  up = open ("WKP", O_RDONLY);
  //down = open ("server_side", O_RDWR); // kinda useless
  printf("pipe opened: %d\n", up);
  read(up, pid, sizeof(pid));
  //semaphores can be used
  remove("WKP");
  *to_client = open(pid, O_WRONLY);
  printf("Server PID given: %d\n", *to_client);

  write(*to_client, pipe1writemessage, sizeof(pipe1writemessage));
  read(*to_client, msg, sizeof(msg));
  printf("MESSAGE: %s\n", msg);


  // char pipe1writemessage[30] = "Hello Server!!";
  // write(pid[1], pipe1writemessage, sizeof(pipe1writemessage));
  //
  // int down;
  // mkfifo("client_side", 0644);
  // printf("pipe created\n");
  // down = open ("client_side", O_RDONLY);
  // //down = open ("client_side", O_RDWR); // kinda useless
  // printf("pipe opened: %d\n", down);
  // //semaphores can be used


  return *to_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int up;
  mkfifo("server_side", 0644);
  printf("pipe created\n");
  up = open ("server_side", O_RDONLY);
  //down = open ("server_side", O_RDWR); // kinda useless
  printf("pipe opened: %d\n", up);
  //semaphores can be used

  int down;
  mkfifo("client_side", 0644);
  printf("pipe created\n");
  down = open ("client_side", O_RDONLY);
  //down = open ("client_side", O_RDWR); // kinda useless
  printf("pipe opened: %d\n", down);
  //semaphores can be used
  *to_server = down;

  // close(to_child_fd[1]);  /* child closes write side of child  pipe */
  // close(to_parent_fd[0]); /* child closes read  side of parent pipe */

  return up;
}
