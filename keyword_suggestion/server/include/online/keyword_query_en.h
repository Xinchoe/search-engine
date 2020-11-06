#pragma once

#include "dict_index_en.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;
using std::vector;
using std::map;
using std::unordered_map;
using std::greater;
using std::pair;
using std::set;
using std::min;

namespace keyword_suggestion
{

    using DictPtr_en=dictAndIndex_en *;

class KeyWordsQuery
{
public:
    KeyWordsQuery(const DictPtr_en & pDictEn);

    vector<string> doQuery(const string &searchWord);//
    void getCandidateWords();//根据搜索词从索引文件中获取候选词
    void getSimilarNumberWords();//从候选词中选similarWordNumber个最小编辑距离的词

    int getWordDistance(const string &lword, const string &rword);//获取两个单词间的最小编辑距离
    int getWordFrequency(const string & word);//获取词频
private:
    string searchWord_;

    vector<vector<string>> candidateWords_;//候选词集合
    vector<string> similarWords_;//相似词集合
    DictPtr_en _pDictEn; 
};

}