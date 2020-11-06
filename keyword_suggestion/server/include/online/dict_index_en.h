#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::set;

namespace keyword_suggestion
{

class dictAndIndex_en
{

public:
    static dictAndIndex_en *getInstance(){
        if(nullptr == _pDictAndIndex_en){
            _pDictAndIndex_en = new dictAndIndex_en();
        }
        return _pDictAndIndex_en;
        
    }
    static void destroy(){
        if(_pDictAndIndex_en){
            delete _pDictAndIndex_en;
            _pDictAndIndex_en = nullptr;
        }
    }

    vector<pair<string,int>> getDictEn() { return dictEn_;}
    unordered_map<char,set<int>> getIndexEn() { return indexEn_;}

    void buildEnDict(const string & filename);//建立字典文件
    void buildEnIndex(const string & filename);//建立索引文件

private:
    dictAndIndex_en() { }
    ~dictAndIndex_en();
private:
    vector<pair<string,int>> dictEn_;//存储字典文件
    unordered_map<char,set<int>> indexEn_;//存储索引文件
    static dictAndIndex_en *_pDictAndIndex_en;
};



}