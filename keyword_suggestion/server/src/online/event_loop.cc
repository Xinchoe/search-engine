#include "../../include/online/event_loop.h"

namespace keyword_suggestion {

EventLoop::EventLoop(Acceptor &acceptor)
    : acceptor_(acceptor),
      event_list_(1024),
      epoll_fd_(CreateEpollFd()),
      is_looping_(false) {
  AddReadFd(acceptor_.GetFd());
}

void EventLoop::Loop() {
  is_looping_ = true;
  while (is_looping_) {
    WaitEpollFd();
  }
}

void EventLoop::Unloop() { is_looping_ = false; }

int EventLoop::CreateEpollFd() {
  int ret = ::epoll_create1(0);

  if (ret == -1) {
    perror("epoll_create1");
  }
  return ret;
}

void EventLoop::SetConnectedCallback(const TcpCallback &&callback) {
  Connected = std::move(callback);
}

void EventLoop::SetReceivedCallback(const TcpCallback &&callback) {
  Received = std::move(callback);
}

void EventLoop::SetClosedCallback(const TcpCallback &&callback) {
  Closed = std::move(callback);
}

void EventLoop::AddReadFd(int fd) {
  struct epoll_event current;
  int ret;

  current.data.fd = fd;
  current.events = EPOLLIN;
  ret = ::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &current);
  if (ret == -1) {
    perror("epoll_ctl");
  }
}

void EventLoop::DeleteReadFd(int fd) {
  struct epoll_event current;
  int ret;

  current.data.fd = fd;
  current.events = EPOLLIN;
  ret = ::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, &current);
  if (ret == -1) {
    perror("epoll_ctl");
  }
}

void EventLoop::WaitEpollFd() {
  int ret;

  do {
    ret = ::epoll_wait(epoll_fd_, &*event_list_.begin(), event_list_.size(),
                       5000);
  } while (ret == -1 && errno == EINTR);
  if (ret == -1) {
    perror("epoll_wait");
    return;
  } else if (!ret) {
    printf("\e[1m[Server]\e[0m\n");
    printf("  Timeout\n");
  } else {
    if (ret == static_cast<int>(event_list_.size())) {
      event_list_.resize(ret * 2);
    }
    for (int i = 0; i != ret; ++i) {
      int fd = event_list_[i].data.fd;

      if (event_list_[i].events & EPOLLIN) {
        if (fd == acceptor_.GetFd()) {
          HandleNewConnection();
        } else {
          HandleMessage(fd);
        }
      }
    }
  }
}

void EventLoop::HandleNewConnection() {
  int peer_fd = acceptor_.Accept();

  AddReadFd(peer_fd);

  TcpPtr connection(new Tcp(peer_fd));

  connection->SetConnectedCallback(Connected);
  connection->SetReceivedCallback(Received);
  connection->SetClosedCallback(Closed);
  connection_.insert(std::make_pair(peer_fd, connection));
  connection->HandleConnectedCallback();
}

void EventLoop::HandleMessage(int fd) {
  auto it = connection_.find(fd);
  auto is_closed = it->second->IsConnectionClosed();

  assert(it != connection_.end());
  if (is_closed) {
    DeleteReadFd(fd);
    it->second->HandleClosedCallback();
    connection_.erase(it);
  } else {
    it->second->HandleReceivedCallback();
  }
}

}  // namespace keyword_suggestion
