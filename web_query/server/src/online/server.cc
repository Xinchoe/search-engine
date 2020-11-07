#include <functional>

#include "../../include/online/configuration.h"
#include "../../include/online/dict_index_en.h"
#include "../../include/online/my_task.h"
#include "../../include/online/tcp_server.h"
#include "../../include/online/thread_pool.h"

keyword_suggestion::ThreadPool *thread_pool_ptr = nullptr;

void Run();
void Connected(const keyword_suggestion::TcpPtr &connection);
void Received(const keyword_suggestion::TcpPtr &connection,
              const keyword_suggestion::DictPtr_cn &dict_cn,
              const keyword_suggestion::DictPtr_en &pdict_en);
void Closed(const keyword_suggestion::TcpPtr &connection);

int main() {
  Run();
  return 0;
}

void Run() {
  keyword_suggestion::ThreadPool thread_pool(4, 10);

  thread_pool.Start();
  thread_pool_ptr = &thread_pool;

  keyword_suggestion::Configuration path("path");
  keyword_suggestion::Configuration net("net");

  keyword_suggestion::Dict_cn *dict_cn =
      keyword_suggestion::Dict_cn::createInstance();

  dict_cn->initDictCn(path.GetPath("dict_zh"));
  dict_cn->initIndexTableCn(path.GetPath("index_zh"));

  keyword_suggestion::dictAndIndex_en *dict_en =
      keyword_suggestion::dictAndIndex_en::getInstance();

  dict_en->buildEnDict(path.GetPath("dict_en"));
  dict_en->buildEnIndex(path.GetPath("index_en"));

  keyword_suggestion::TcpServer tcp_server(net.GetPort("default_port"), net.GetIp("default_ip"));

  tcp_server.SetConnectedCallback(Connected);
  tcp_server.SetReceivedCallback(
      std::bind(Received, std::placeholders::_1, dict_cn, dict_en));
  tcp_server.SetClosedCallback(Closed);
  tcp_server.Start();
}

void Connected(const keyword_suggestion::TcpPtr &connection) {
  keyword_suggestion::Message msg;

  printf("\e[1m[Server]\e[0m\n");
  printf("  %s has connected\n", connection->ConvertToString().c_str());
  strcpy(msg.content, "Welcome");
  msg.len = strlen(msg.content);
  msg.id = 0;
  connection->Send(msg);
}

void Received(const keyword_suggestion::TcpPtr &connection,
              const keyword_suggestion::DictPtr_cn &pdict,
              const keyword_suggestion::DictPtr_en &pdict_en) {
  keyword_suggestion::Message msg;

  memset(&msg, 0, sizeof(msg));
  msg = connection->Receive();

  printf("\e[1;36m[Client]\e[0m\n");
  printf("  %s\n", msg.content);

  keyword_suggestion::MyTask task(msg, connection, pdict, pdict_en);
  thread_pool_ptr->AddTask(
      std::bind(&keyword_suggestion::MyTask::execute, task));
}

void Closed(const keyword_suggestion::TcpPtr &connection) {
  printf("\e[1m[Server]\e[0m\n");
  printf("  The connection is closed\n");
}
