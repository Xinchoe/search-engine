#include "../include/socket_io.h"

namespace keyword_suggestion {

SocketIo::SocketIo(int fd) : fd_(fd) {}

int SocketIo::ReadALine(char *buffer, int size) {
  int ret, total = 0, offset = size - 1;
  char *ptr = buffer;

  while (offset > 0) {
    ret = ReceivePeek(ptr, offset);
    for (int i = 0; i < ret; i++) {
      if (*(ptr + i) == '\n') {
        ReadN(ptr, i + 1);
        total += i + 1;
        *(ptr + i) = '\0';
        return total;
      }
    }
    ReadN(ptr, ret);
    offset -= ret;
    ptr += ret;
    total += ret;
  }
  *ptr = '\0';
  return total;
}

int SocketIo::ReadN(char *buffer, int n) {
  int ret, offset = n;
  char *ptr = buffer;

  while (offset > 0) {
    ret = ::read(fd_, ptr, offset);
    if (ret == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("read");
      return n - offset;
    } else if (!ret) {
      return n - offset;
    } else {
      offset -= ret;
      ptr += ret;
    }
  }
  return n - offset;
}

int SocketIo::WriteN(const char *buffer, int n) {
  int ret, offset = n;
  const char *ptr = buffer;

  while (offset > 0) {
    ret = ::write(fd_, ptr, offset);
    if (ret == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("write");
      return n - offset;
    } else {
      offset -= ret;
      ptr += ret;
    }
  }
  return n - offset;
}

int SocketIo::ReceivePeek(char *buffer, int n) {
  int ret;

  do {
    ret = ::recv(fd_, buffer, n, MSG_PEEK);
  } while (ret == -1 && errno == EINTR);
  return ret;
}

}  // namespace keyword_suggestion
