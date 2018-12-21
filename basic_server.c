#include "pipe_networking.h"

static void sighandler(int signo){
  // if ctrl + C is pressed the wkp is removed and program is exitted
  if(signo == SIGINT){
    remove("WKP");
    exit(0);
  }
}

int main() {
  // checks for signals
  signal(SIGINT, sighandler);

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
}
