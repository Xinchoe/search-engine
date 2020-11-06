#include "../../include/online/keyword_query_en.h"

#define similarWordNumber 5 //相似词个数

namespace keyword_suggestion
{

dictAndIndex_en *dictAndIndex_en::_pDictAndIndex_en = nullptr;

KeyWordsQuery::KeyWordsQuery(const DictPtr_en & dictEn)
:_pDictEn(dictEn)
{
    vector<string> v1,v2,v3,v4,v5;
    candidateWords_.push_back(v1);
    candidateWords_.push_back(v2);
    candidateWords_.push_back(v3);
    candidateWords_.push_back(v4);
    candidateWords_.push_back(v5);
    similarWords_.reserve(similarWordNumber);
}

vector<string> KeyWordsQuery::doQuery(const string &searchWord)
{
    
    searchWord_ = searchWord;
    getCandidateWords();//从索引文件中获取候选词
    getSimilarNumberWords();//从候选词中选similarWordNumber个最小编辑距离的词
    return similarWords_;
}

//从索引文件中获取候选词,存入candidateWords_
void KeyWordsQuery::getCandidateWords()
{
    set<int> candidateID;
    int distance = 0;
    unordered_map<char,set<int>> currenIndex = _pDictEn->getIndexEn();
    vector<pair<string,int>> currenDict = _pDictEn->getDictEn();

    for(char c : searchWord_) {
        auto iter = currenIndex.find(c);
        for(int i : iter->second){
            candidateID.insert(i);
        }
    }

    int count = 0;
    for(auto iter = candidateID.begin(); iter != candidateID.end(); ++iter){
        string candidateWord = currenDict[*iter].first;
        distance = getWordDistance(searchWord_,candidateWord);
        if(distance < 5){
            candidateWords_[distance].push_back(candidateWord);
        }
    }
}

//从候选词中选similarWordNumber个最小编辑距离的词,存入similarWords_
void KeyWordsQuery::getSimilarNumberWords()
{
    int distance = 1;

    while(similarWords_.size() != similarWordNumber) {
        if(candidateWords_[distance].size() + similarWords_.size() <= similarWordNumber) {
            for(string word : candidateWords_[distance]) {
                similarWords_.push_back(word);
            }
            ++distance;
        } else {
            int count = similarWordNumber - similarWords_.size();
            //从candidateWords_[distance]中找到
            //词频最大的count个单词并存入similarWords_ 
            map<int,string,greater<int>> findCountWords;
            for(string word : candidateWords_[distance]){
                findCountWords.insert({getWordFrequency(word),word}); 
            }

            auto iter = findCountWords.begin();
            while(count > 0){
                similarWords_.push_back(iter->second);
                ++iter;
                --count;
            }
        }
    }
}

int KeyWordsQuery::getWordDistance(const string &word1,const string &word2)
{
    int n = word1.length();
    int m = word2.length();

    // 有一个字符串为空串
    if (n * m == 0) return n + m;

    // DP 数组
    int D[n + 1][m + 1];

    // 边界状态初始化
    for (int i = 0; i < n + 1; i++) {
        D[i][0] = i;
    }
    for (int j = 0; j < m + 1; j++) {
        D[0][j] = j;
    }

    // 计算所有 DP 值
    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < m + 1; j++) {
            int left = D[i - 1][j] + 1;
            int down = D[i][j - 1] + 1;
            int left_down = D[i - 1][j - 1];
            if (word1[i - 1] != word2[j - 1]) left_down += 1;
            D[i][j] = min(left, min(down, left_down));
        }
    }
    return D[n][m];
}

int KeyWordsQuery::getWordFrequency(const string & word)
{
    vector<pair<string,int>> currenDict = _pDictEn->getDictEn();
    for(auto val : currenDict){
        if(val.first == word){
            return val.second;
        }
    }
    return -1;
}

}
