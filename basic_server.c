#include "pipe_networking.h"

static void sighandler(int signo){
  // if ctrl + C is pressed the wkp is removed and program is exitted
  if(signo == SIGINT){
    remove("WKP");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);

  int to_client;
  int from_client;

  char msg[BUFFER_SIZE];

  while(1){
    server_handshake_part1(&from_client);
    int client_pid = fork();
    if(client_pid){
      close(from_client);
      //printf("removing wkp\n");
      remove("wkp");
      //printf("removed wkp\n");
    }
    else{
      server_handshake_part2(&to_client, &from_client);
      while(read(from_client, msg, sizeof(msg))){
	printf("%d: client data: %s", getpid(), msg);
  strcat(msg, "        ~Samuel Konstantinovich 2k18");
  printf("SECOND MESSAGE: %s\n", msg);	//printf("%s", msg);
	write(to_client, msg, sizeof(msg));
	printf("%d: processed data: %s", getpid(), msg);
      }
    }
  }
  return 0;
}
