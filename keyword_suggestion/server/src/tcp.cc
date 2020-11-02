#include "../include/tcp.h"

namespace reactor {

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

void Tcp::Send(const std::string &msg) {
  socket_io_.WriteN(msg.c_str(), msg.size());
}

std::string Tcp::Receive() {
  char buffer[kMaxSize] = {0};

  socket_io_.ReadALine(buffer, sizeof(buffer));
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

bool Tcp::IsConnectionClosed() {
  int ret;
  char buffer[kMaxSize];

  do {
    ret = ::recv(socket_.fd(), buffer, sizeof(buffer), MSG_PEEK);
  } while (ret == -1 && errno == EINTR);
  return ret == 0;
}

void Tcp::SetConnectedCallback(const TcpCallback &callback) {
  Connected = callback;
}

void Tcp::SetReceivedCallback(const TcpCallback &callback) {
  Received = callback;
}

void Tcp::SetClosedCallback(const TcpCallback &callback) { Closed = callback; }

void Tcp::HandleConnectedCallback() {
  if (Connected) {
    Connected(shared_from_this());
  }
}

void Tcp::HandleReceivedCallback() {
  if (Received) {
    Received(shared_from_this());
  }
}

void Tcp::HandleClosedCallback() {
  if (Closed) {
    Closed(shared_from_this());
  }
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

}  // namespace reactor
