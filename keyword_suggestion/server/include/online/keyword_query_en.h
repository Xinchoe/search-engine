#ifndef KEYWORD_SUGGESTION_SERVER_INCLUDE_ONLINE_KEYWORD_QUERY_EN_H_
#define KEYWORD_SUGGESTION_SERVER_INCLUDE_ONLINE_KEYWORD_QUERY_EN_H_

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "dict_index_en.h"

using std::cout;
using std::endl;
using std::greater;
using std::ifstream;
using std::map;
using std::min;
using std::pair;
using std::set;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

namespace keyword_suggestion {

using DictPtr_en = dictAndIndex_en *;

class KeyWordsQuery {
 public:
  KeyWordsQuery(const DictPtr_en &pDictEn);

  vector<string> doQuery(const string &searchWord);  //
  void getCandidateWords();  //根据搜索词从索引文件中获取候选词
  void
  getSimilarNumberWords();  //从候选词中选similarWordNumber个最小编辑距离的词

  int getWordDistance(const string &lword,
                      const string &rword);  //获取两个单词间的最小编辑距离
  int getWordFrequency(const string &word);  //获取词频
 private:
  string searchWord_;

  vector<vector<string>> candidateWords_;  //候选词集合
  vector<string> similarWords_;            //相似词集合
  DictPtr_en _pDictEn;
};

}  // namespace keyword_suggestion

#endif
