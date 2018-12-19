#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int up = 0;
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];
  // char pipe1writemessage[30] = "Hello Server!!";


  mkfifo("WKP", 0644);
  printf("WKP pipe created and opened\n");
  up = open ("WKP", O_RDONLY);
  //down = open ("server_side", O_RDWR); // kinda useless
  printf("pipe opened: %d\n", up);

  read(up, pid, sizeof(pid));
  //semaphores can be used
  remove("WKP");

  // opens private pipe
  *to_client = open(pid, O_WRONLY);
  printf("Server PID given: %d\n", *to_client);
  // writes message to private pipe for client
  write(*to_client, ACK, sizeof(ACK));

  // reads client's response from private pipe
  read(up, msg, sizeof(msg));
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

  int down = 0;
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];
  // char pipe1writemessage[30] = "Hello Client!!";
  sprintf(pid, "%d", getpid());

  mkfifo(pid, 0644);
  printf("pipe created pid\n");
  *to_server = open ("WKP", O_WRONLY);
  //down = open ("server_side", O_RDWR); // kinda useless
  printf("pipe opened: %d\n", *to_server);

  // Client wrote to WKP
  write(*to_server, pid, sizeof(pid));

  // Client opens private pipe
  down = open(pid, O_RDONLY);
  // reads message from server
  read(down, msg, sizeof(msg));
  printf("CLIENT READ SERVER'S MESSAGE '%s'\n", msg);

  // removing private pipe
  remove(pid);

  // writes message from client to server using WKP
  write(*to_server, ACK, sizeof(ACK));
  printf("CLIENT WROTE '%s' TO SERVER\n", ACK);

  return down;
}
