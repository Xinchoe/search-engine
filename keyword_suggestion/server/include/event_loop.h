#ifndef INCLUDE_EVENT_LOOP_H_
#define INCLUDE_EVENT_LOOP_H_

#include <assert.h>
#include <stdio.h>
#include <sys/epoll.h>

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "acceptor.h"
#include "tcp.h"

namespace reactor {

using TcpPtr = std::shared_ptr<Tcp>;
using TcpCallback = std::function<void(const TcpPtr &connection)>;

class EventLoop {
 public:
  EventLoop(Acceptor &);

  void Loop();
  void Unloop();

  void SetConnectedCallback(const TcpCallback &&callback);
  void SetReceivedCallback(const TcpCallback &&callback);
  void SetClosedCallback(const TcpCallback &&callback);

 private:
  int CreateEpollFd();
  void AddReadFd(int fd);
  void DeleteReadFd(int fd);
  void WaitEpollFd();
  void HandleNewConnection();
  void HandleMessage(int fd);

  TcpCallback Connected;
  TcpCallback Received;
  TcpCallback Closed;

  Acceptor &acceptor_;
  std::vector<struct epoll_event> event_list_;
  std::map<int, TcpPtr> connection_;
  int epoll_fd_;
  bool is_looping_;
};

}  // namespace reactor

#endif
