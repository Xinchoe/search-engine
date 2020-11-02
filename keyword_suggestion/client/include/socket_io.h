#ifndef INCLUDE_SOCKET_IO_H_
#define INCLUDE_SOCKET_IO_H_

#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

namespace reactor {

class SocketIo {
 public:
  explicit SocketIo(int);

  int ReadALine(char *, int);
  int ReadN(char *, int);
  int WriteN(const char *, int);

 private:
  int ReceivePeek(char *, int);

  int fd_;
};

}  // namespace reactor

#endif
