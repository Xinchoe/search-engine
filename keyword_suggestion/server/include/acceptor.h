#ifndef INCLUDE_ACCEPTOR_H_
#define INCLUDE_ACCEPTOR_H_

#include <string>

#include "inet_address.h"
#include "socket.h"

const int kBacklog = 8;

namespace reactor {

class Acceptor {
 public:
  Acceptor(int port, const std::string &ip = "0.0.0.0");

  void Init();
  int Accept();
  int GetFd() const;

 private:
  void SetReusePort(bool arg);
  void SetReuseAddr(bool arg);
  void Bind();
  void Listen();

  InetAddress addr_;
  Socket socket_;
};

}  // namespace reactor

#endif
