#include "../../include/offline/split_tool_cppjieba.h"
#include "../../include/offline/cppjieba-master/include/Jieba.hpp"
#include <locale>
#include <codecvt>

const char* const DICT_PATH = "../../include/offline/cppjieba-master/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../../include/offline/cppjieba-master/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../../include/offline/cppjieba-master/dict/user.dict.utf8";
const char* const IDF_PATH = "../../include/offline/cppjieba-master/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../../include/offline/cppjieba-master/dict/stop_words.utf8";
using namespace cppjieba;
namespace keyword_suggestion
{

void handleUtf8String(string &s)
{
    locale loc("en_US.UTF-8");
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    wstring ws=conv.from_bytes(s);
    wstring nws;
    for(wchar_t &ch:ws)
    {
        if(!ispunct(ch,loc)&&!iswspace(ch)&&!iswalnum(ch))
        {
            nws.push_back(ch);
        }
    }
    s=conv.to_bytes(nws);
    /* for(auto i=s.begin();i!=s.end();) */
    /* { */
    /*     if(iswalnum(*i)) */
    /*     { */
    /*         i=s.erase(i); */
    /*     } */
    /*     else */
    /*     { */
    /*         ++i; */
    /*     } */
    /* } */
}

vector<string> SplitToolCppJieba::cut(const string &sentence)
{
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH);
    vector<string> words;
    string s;
    s=sentence;
    vector<KeywordExtractor::Word> keywordres;
    int topk=10000;
    jieba.extractor.Extract(s,keywordres,topk);
    /* handleUtf8String(s); */
    /* cout<<s<<endl; */
    /* jieba.CutForSearch(s,words); */
    for(size_t i=0;i<keywordres.size();++i)
    {
        words.push_back(keywordres[i].word);
    }
    //提取关键词之后将数字与字母从字典中去除
    for(auto i=words.begin();i!=words.end();)
    {
        if(isalnum((*i)[0]))
        {
            i=words.erase(i);
        }
        else
        {
            ++i;
        }
    }
    return words;
}
}//end of namespace keyword_suggestion
