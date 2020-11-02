#ifndef INCLUDE_SOCKET_H_
#define INCLUDE_SOCKET_H_

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

namespace reactor {

class Socket {
 public:
  Socket();
  explicit Socket(int fd);
  ~Socket();

  void ShutdownWrite();

  int fd();

 private:
  int fd_;
};

}  // namespace reactor

#endif
