#include "../include/connector.h"
#include "../include/tcp.h"

void Run();

int main() {
  Run();
  return 0;
}

void Run() {
  reactor::Connector connector(1024, "192.168.254.163");
  reactor::Tcp tcp(connector.Connect());

  printf("\e[1m[Client]\e[0m\n");
  printf("  %s has connected\n", tcp.ConvertToString().c_str());
  while (1) {
    printf("\e[1;36m[Server]\e[0m\n");
    printf("  %s\n", tcp.Receive().c_str());
    printf("\e[1;32m[Client]\e[0m\n  ");
    tcp.Send();
  }
}
