#ifndef _MYTASK_H_
#define _MYTASK_H_

#include "./../tcp.h"
#include "dict_cn.h"
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <set>

using std::priority_queue;
using std::string;
using std::vector;
using std::set;

namespace keyword_suggestion
{
using DictPtr_cn=Dict_cn *;


struct MyResult
{
    friend struct MyCompare;
    string _word;
    int _iFreq;
    int _iDist;
    MyResult(const string & word,int freq,int dist)
        :_word(word)
        ,_iFreq(freq)
        ,_iDist(dist)
    {}
};

struct MyCompare
{
    bool operator()(const MyResult & lhs,const MyResult & rhs)
    {
        if(lhs._iDist!=rhs._iDist)
        {
            return lhs._iDist>rhs._iDist;
        }
        else 
        {
            if(lhs._iFreq!=rhs._iFreq)
            {
                return lhs._iFreq>rhs._iFreq;
            }
            else
            {
                return lhs._word>rhs._word;
            }
        }
    }
};

class MyTask
{
public:
    MyTask(const string & queryWord,const keyword_suggestion::TcpPtr & pConn,const DictPtr_cn & pDictCn);
    
    void execute();  //执行查询

private:
    void statistic(set<int> & iset);  //进行计算
    int distance(const string & rhs);  //最小编辑距离
    void response();  //响应客户端请求
    size_t nBytesCode(const char ch);
    size_t length(const string &str);
    void parseWord(vector<string> & chars);

private:
    string _queryWord;
    keyword_suggestion::TcpPtr _pConn;
    DictPtr_cn _pDictCn; 
    priority_queue<MyResult,vector<MyResult>,MyCompare> _resultQue;


};
} //end of namespace keyword_suggestion
#endif
