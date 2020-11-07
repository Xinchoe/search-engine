#ifndef KEYWORD_SUGGESTION_SERVER_INCLUDE_ONLINE_MY_TAKS_H_
#define KEYWORD_SUGGESTION_SERVER_INCLUDE_ONLINE_MY_TAKS_H_

#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "dict_cn.h"
#include "dict_index_en.h"
#include "tcp.h"

using std::priority_queue;
using std::set;
using std::string;
using std::vector;

namespace keyword_suggestion {
using DictPtr_cn = Dict_cn *;
using DictPtr_en = dictAndIndex_en *;

struct MyResult {
  friend struct MyCompare;
  string _word;
  int _iFreq;
  int _iDist;
  MyResult(const string &word, int freq, int dist)
      : _word(word), _iFreq(freq), _iDist(dist) {}
};

struct MyCompare {
  bool operator()(const MyResult &lhs, const MyResult &rhs) {
    if (lhs._iDist != rhs._iDist) {
      return lhs._iDist > rhs._iDist;
    } else {
      if (lhs._iFreq != rhs._iFreq) {
        return lhs._iFreq > rhs._iFreq;
      } else {
        return lhs._word > rhs._word;
      }
    }
  }
};

class MyTask {
 public:
  MyTask(Message &msg, const keyword_suggestion::TcpPtr &pConn,
         const DictPtr_cn &pDictCn, const DictPtr_en &pDictEn);

  void execute();  //执行查询

 private:
  void statistic(set<int> &iset);   //进行计算
  int distance(const string &rhs);  //最小编辑距离
  /* void response();  //响应客户端请求 */
  size_t nBytesCode(const char ch);
  size_t length(const string &str);
  void parseWord(vector<string> &chars);

  string packetJson_zh();
  string packetJson_en(const vector<string> &vec);
  void packetMessage(string result);

 private:
  Message msg_;
  std::string _queryWord;
  keyword_suggestion::TcpPtr _pConn;
  DictPtr_cn _pDictCn;
  priority_queue<MyResult, vector<MyResult>, MyCompare> _resultQue;
  DictPtr_en _pDictEn;
};
}  // end of namespace keyword_suggestion

#endif
