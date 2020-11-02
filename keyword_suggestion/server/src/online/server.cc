#include "../../include/online/tcp_server.h"

void Run();
void Connected(const keyword_suggestion::TcpPtr &connection);
void Received(const keyword_suggestion::TcpPtr &connection);
void Closed(const keyword_suggestion::TcpPtr &connection);

int main() {
  Run();
  return 0;
}

void Run() {
  keyword_suggestion::TcpServer tcp_server(1024, "192.168.254.163");

  tcp_server.SetConnectedCallback(Connected);
  tcp_server.SetReceivedCallback(Received);
  tcp_server.SetClosedCallback(Closed);
  tcp_server.Start();
}

void Connected(const keyword_suggestion::TcpPtr &connection) {
  printf("\e[1m[Server]\e[0m\n");
  printf("  %s has connected\n", connection->ConvertToString().c_str());
  connection->Send("Welcome\n");
}

void Received(const keyword_suggestion::TcpPtr &connection) {
  std::string msg = connection->Receive();

  printf("\e[1;36m[Client]\e[0m\n");
  printf("  %s\n", msg.c_str());
  msg = "Message \"" + msg + "\" has been received\n";
  connection->Send(msg);
}

void Closed(const keyword_suggestion::TcpPtr &connection) {
  printf("\e[1m[Server]\e[0m\n");
  printf("  The connection is closed\n");
}
