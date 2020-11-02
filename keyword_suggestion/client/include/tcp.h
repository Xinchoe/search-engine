#ifndef INCLUDE_TCP_H_
#define INCLUDE_TCP_H_

#include <iostream>
#include <sstream>
#include <string>

#include "inet_address.h"
#include "noncopyable.h"
#include "socket.h"
#include "socket_io.h"

const size_t kMaxSize = 65536;

namespace keyword_suggestion {

class Tcp : Noncopyable {
 public:
  Tcp(int);
  ~Tcp();

  void Send();
  std::string Receive();
  std::string ConvertToString() const;
  void Shutdown();

 private:
  InetAddress GetSockAddr(int fd);
  InetAddress GetPeerAddr(int fd);

  Socket socket_;
  SocketIo socket_io_;
  InetAddress sock_addr_;
  InetAddress peer_addr_;
  bool is_shutdown_;
};

}  // namespace keyword_suggestion

#endif
