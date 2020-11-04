#ifndef INCLUDE_SOCKET_IO_H_
#define INCLUDE_SOCKET_IO_H_

#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

namespace keyword_suggestion {

class SocketIo {
 public:
  explicit SocketIo(int fd);

  int ReadALine(char *buffer, int n);
  int ReadN(char *buffer, int n);
  int WriteN(const char *buffer, int n);

 private:
  int ReceivePeek(char *buffer, int n);

  int fd_;
};

}  // namespace keyword_suggestion

#endif
