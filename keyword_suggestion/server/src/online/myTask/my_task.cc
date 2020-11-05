#include "../../../include/online/myTask/my_task.h"
#include "../../../include/online/myTask/message.h"
#include <algorithm>
#include <json/json.h>
#include <string.h>

namespace keyword_suggestion
{

MyTask::MyTask(const string & queryWord,const TcpPtr & pConn,const DictPtr_cn & dictCn)
    :_queryWord(queryWord)
    ,_pConn(pConn)
    ,_pDictCn(dictCn)
    {}

//判断字符的字节数/区分查询词是中文还是英文
size_t MyTask::nBytesCode(const char ch)
{
    if(ch & (1<<7)) //字节的最高位是否为1  
    {
        int nByte=1;
        for(int idx=0;idx!=6;++idx) //utf-8最大6个字节，字节的高位最多6个表示字节数的1
        {
            if(ch & (1<<(6-idx)))
            {
                ++nByte;
            }
            else
            {
                break;
            }
        }
        return nByte;
    }
    return 1;
}
//返回字符串的字符长
size_t MyTask::length(const string &str)
{
    int len=0;
    for(size_t idx=0;idx!=str.size();)
    {
        size_t nByte=nBytesCode(str[idx]);
        idx+=nByte;
        ++len;
    }
    return len;
}

//最小编辑距离（中英文都合适）
int MyTask::distance(const string &rhs)
{
    int lhs_len=length(_queryWord); //字符长
    int rhs_len=length(rhs);
    int blhs_len=_queryWord.size(); //字节长
    int brhs_len=rhs.size();

    //如果lhs或rhs为空串
    if(lhs_len*rhs_len==0)
    {
        return lhs_len+rhs_len;
    }

    int dp[lhs_len+1][rhs_len+1];  //dp[i][j]代表 lhs前i个字符组成的子串转化为rhs前j个字符组成的子串
    
    //初始化
    for(int idx=0;idx<lhs_len+1;++idx)
    {
        dp[idx][0]=idx;
    }
    for(int idx=0;idx<rhs_len+1;++idx)
    {
        dp[0][idx]=idx;
    }

    string sublhs,subrhs;
    for(int i=1,lhs_idx=0;i<lhs_len+1&&lhs_idx<blhs_len;++i)
    {
        size_t nByte=nBytesCode(_queryWord[lhs_idx]);
        sublhs=_queryWord.substr(lhs_idx,nByte);
        lhs_idx+=nByte;
        
        for(int j=1,rhs_idx=0;j<rhs_len+1&&rhs_idx<brhs_len;++j)
        {
            size_t nByte=nBytesCode(rhs[rhs_idx]);
            subrhs=rhs.substr(rhs_idx,nByte);
            rhs_idx+=nByte;

            if(sublhs==subrhs)
            {
                dp[i][j]=std::min(dp[i-1][j]+1,std::min(dp[i][j-1]+1,dp[i-1][j-1]));
            }
            else
            {
                dp[i][j]=std::min(dp[i-1][j]+1,std::min(dp[i][j-1]+1,dp[i-1][j-1]+1));
            }
        }
    }
    return dp[lhs_len][rhs_len];
}

void MyTask::response()
{
    //序列化
    Json::Value arrayObj;

    int sz=_resultQue.size();
    if(sz<5)
    {
        for(int i=0;i<sz;++i)
        {
            Json::Value item;
            string candidate=_resultQue.top()._word;
            _resultQue.pop();
            item["candidate"]=candidate;
            arrayObj.append(item);
        }
    }
    else
    {
        for(int i=0;i<5;++i)  //返回优先级队列的前3个元素
        {
            Json::Value item;
            string candidate=_resultQue.top()._word;
            _resultQue.pop();
            item["candidate"]=candidate;
            arrayObj.append(item);
        }
    
    }

    Json::FastWriter writer;

    string result=writer.write(arrayObj);

    /* std::cout<<"MyTask::result="<<result<<std::endl; */
    //消息协议
    Message mgs;
    mgs._id=100;
    mgs._len=result.size();
    /* std::cout<<"result.size()="<<mgs._len<<std::endl; */
    strcpy(mgs._content,result.c_str());
    //交给主线程
    _pConn->SendInLoop(mgs);

}
void MyTask::parseWord(vector<string> & chars)
{
    for(size_t idx=0;idx!=_queryWord.size();)
    {
        int nByte=nBytesCode(_queryWord[idx]);
        chars.push_back(_queryWord.substr(idx,nByte));
        idx+=nByte;
    }
}

void MyTask::execute()
{
    //1.判断查询词是中文还是英文
    int sz=nBytesCode(_queryWord[0]);
    if(sz>1)
    {
        //2.对查询词进行分词
        vector<string> chars;  //如，查询词：一家人，分词以后chars里分别存放：一 家 人
        parseWord(chars);
        
        unordered_map<string,set<int>> indexTableCn=_pDictCn->getIndexTableCn();

        //3.遍历chars,查找每一个字符的关联词
        set<int> iset;
        for(size_t idx=0;idx!=chars.size();++idx)
        {
            auto cnt=indexTableCn.count(chars[idx]); //chars[idx] :具体某一个字
            if(1==cnt)
            {
                set<int> sets=indexTableCn[chars[idx]]; //返回该字相关联词的词典下标集合
                std::copy(sets.begin(),sets.end(),std::insert_iterator<set<int>>(iset,iset.begin()));
            }
        }
        statistic(iset);
        response();
    }
}
//计算每一个关联词与查询词的距离，并插入优先级队列
void MyTask::statistic(set<int>  & iset)
{
    vector<std::pair<string,int>> dictCn=_pDictCn->getDictCn();
    
    for(auto & idx:iset)
    {
        int dist=distance(dictCn[idx].first);
        _resultQue.push(MyResult(dictCn[idx].first,dictCn[idx].second,dist));
    }
}

}  //end of namespace keyword_suggestion