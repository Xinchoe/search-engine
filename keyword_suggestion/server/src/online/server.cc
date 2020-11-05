#include "../../include/online/tcp_server.h"
#include "../../include/online/thread_pool.h"
#include "../../include/online/myTask/my_task.h"
#include "../../include/online/myTask/configuration.h"
#include <functional>

keyword_suggestion::ThreadPool *thread_pool_ptr = nullptr;

/* class Task { */
/*  public: */
/*   Task(const std::string &msg, const keyword_suggestion::TcpPtr &connection) */
/*       : msg_(msg), connection_(connection) {} */

/*   void Process() { */
/*     printf("\e[1m[Server]\e[0m\n"); */
/*     printf("  ThreadPool's sub thread: %s\n", */
/*            keyword_suggestion::current_thread::name); */
/*     connection_->SendInLoop(msg_); */
/*   } */

/*  private: */
/*   std::string msg_; */
/*   keyword_suggestion::TcpPtr connection_; */
/* }; */

void Run();
void Connected(const keyword_suggestion::TcpPtr &connection);
void Received(const keyword_suggestion::TcpPtr &connection,const keyword_suggestion::DictPtr_cn & dict_cn );
void Closed(const keyword_suggestion::TcpPtr &connection);

int main() {
  Run();
  return 0;
}

void Run() {
  keyword_suggestion::ThreadPool thread_pool(4, 10);
  thread_pool.Start();
  thread_pool_ptr = &thread_pool;

  keyword_suggestion::Configuration conf("path");

  keyword_suggestion::Dict_cn * dict_cn=keyword_suggestion::Dict_cn::createInstance();
  dict_cn->initDictCn(conf.GetContent("dict_zh"));
  dict_cn->initIndexTableCn(conf.GetContent("index_zh"));

  keyword_suggestion::TcpServer tcp_server(2000, "192.168.75.128");

  tcp_server.SetConnectedCallback(Connected);
  tcp_server.SetReceivedCallback(std::bind(Received,std::placeholders::_1,dict_cn));
  tcp_server.SetClosedCallback(Closed);
  tcp_server.Start();
}

void Connected(const keyword_suggestion::TcpPtr &connection) {
  printf("\e[1m[Server]\e[0m\n");
  printf("  %s has connected\n", connection->ConvertToString().c_str());
  /* connection->Send("Welcome\n"); */
}

void Received(const keyword_suggestion::TcpPtr &connection,const keyword_suggestion::DictPtr_cn & pdict) {
  std::string msg = connection->Receive();

  printf("\e[1;36m[Client]\e[0m\n");
  printf("  %s\n", msg.c_str());

  keyword_suggestion::MyTask task(msg, connection,pdict);
  thread_pool_ptr->AddTask(std::bind(&keyword_suggestion::MyTask::execute, task));
}

void Closed(const keyword_suggestion::TcpPtr &connection) {
  printf("\e[1m[Server]\e[0m\n");
  printf("  The connection is closed\n");
}
