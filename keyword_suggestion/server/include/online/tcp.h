#ifndef INCLUDE_TCP_H_
#define INCLUDE_TCP_H_

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "inet_address.h"
#include "noncopyable.h"
#include "socket.h"
#include "socket_io.h"

const size_t kMaxSize = 65536;

namespace reactor {

class Tcp;
using TcpPtr = std::shared_ptr<Tcp>;
using TcpCallback = std::function<void(const TcpPtr &connection)>;

class Tcp : Noncopyable, public std::enable_shared_from_this<Tcp> {
 public:
  Tcp(int);
  ~Tcp();

  void Send(const std::string &msg);
  std::string Receive();
  std::string ConvertToString() const;
  void Shutdown();
  bool IsConnectionClosed();

  void SetConnectedCallback(const TcpCallback &callback);
  void SetReceivedCallback(const TcpCallback &callback);
  void SetClosedCallback(const TcpCallback &callback);
  void HandleConnectedCallback();
  void HandleReceivedCallback();
  void HandleClosedCallback();

 private:
  InetAddress GetSockAddr(int fd);
  InetAddress GetPeerAddr(int fd);

  TcpCallback Connected;
  TcpCallback Received;
  TcpCallback Closed;

  Socket socket_;
  SocketIo socket_io_;
  InetAddress sock_addr_;
  InetAddress peer_addr_;
  bool is_shutdown_;
};

}  // namespace reactor

#endif
