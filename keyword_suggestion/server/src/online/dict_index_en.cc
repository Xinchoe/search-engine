#include "../../include/online/dict_index_en.h"

#define similarWordNumber 5 //相似词个数

namespace keyword_suggestion
{

void dictAndIndex_en::buildEnDict(const string & filename)
{
    string line;
    ifstream ifs(filename);
    if(!ifs.good())
    {
        std::cout<<"the ifstream "<< filename <<" opened error"<<std::endl;
        return;
    }
    while(getline(ifs,line)){
        stringstream ss;
        ss << line << endl;
        string word;
        int frequency;
        while(ss >> word >> frequency, !ss.eof()){
            dictEn_.push_back({word,frequency});
        }
    }
}

void dictAndIndex_en::buildEnIndex(const string & filename)
{
    string line;
    ifstream ifs(filename);
    if(!ifs.good())
    {
        std::cout<<"the ifstream "<< filename <<" opened error"<<std::endl;
        return;
    }
    while(getline(ifs,line)){
        stringstream ss;
        char c;
        int ID;
        set<int> IDs;

        ss << line << endl;
        ss >> c;
        while(ss >> ID, !ss.eof()){
            IDs.insert(ID);
        }
        indexEn_.insert({c,IDs});
    }
}

}