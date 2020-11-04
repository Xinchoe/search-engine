#include "../include/tcp.h"

namespace keyword_suggestion {

Tcp::Tcp(int fd)
    : socket_(fd),
      socket_io_(fd),
      sock_addr_(GetSockAddr(fd)),
      peer_addr_(GetPeerAddr(fd)),
      is_shutdown_(false) {}

Tcp::~Tcp() {
  if (!is_shutdown_) {
    Shutdown();
  }
}

void Tcp::Send() {
  char buffer[kMaxSize] = {0};

  fgets(buffer, sizeof(buffer), stdin);
  socket_io_.WriteN(buffer, strlen(buffer));
}

std::string Tcp::Receive() {
  char buffer[kMaxSize] = {0};

  int ret = socket_io_.ReadALine(buffer, sizeof(buffer));
  return static_cast<std::string>(buffer);
}

std::string Tcp::ConvertToString() const {
  std::ostringstream oss;

  oss << sock_addr_.GetIp() << ":" << sock_addr_.GetPort() << " --> "
      << peer_addr_.GetIp() << ":" << peer_addr_.GetPort();
  return oss.str();
}

void Tcp::Shutdown() {
  is_shutdown_ = true;
  socket_.ShutdownWrite();
}

InetAddress Tcp::GetSockAddr(int fd) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);

  if (getsockname(socket_.fd(), reinterpret_cast<struct sockaddr *>(&addr),
                  &len) == -1) {
    perror("getsockname");
  }
  return InetAddress(addr);
}

InetAddress Tcp::GetPeerAddr(int fd) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr);

  if (getpeername(socket_.fd(), reinterpret_cast<struct sockaddr *>(&addr),
                  &len) == -1) {
    perror("getpeername");
  }
  return InetAddress(addr);
}

}  // namespace keyword_suggestion
