#include "../../../include/online/myTask/dict_cn.h"
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;

namespace keyword_suggestion
{
Dict_cn * Dict_cn::_pInstance=Dict_cn::createInstance();

Dict_cn::AutoRelease  Dict_cn::_auto;

 Dict_cn *  Dict_cn::createInstance()
{
    if(_pInstance==nullptr)
    {
        _pInstance=new Dict_cn();
    }
    return _pInstance;
}

void  Dict_cn::initDictCn(const string & dictCnPath)
{
    ifstream ifs(dictCnPath);
    if(!ifs.good())
    {
        std::cout<<"the ifstream "<<dictCnPath<<" opened error"<<std::endl;
        return;
    }
    string line;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        string word;
        int freq;
        iss>>word;
        iss>>freq;
        _dictCn.push_back(std::make_pair(word,freq));
    }
}

void  Dict_cn::initIndexTableCn(const string & indexTableCnPath)
{
    
    ifstream ifs(indexTableCnPath);
    if(!ifs.good())
    {
        std::cout<<"the ifstream "<<indexTableCnPath<<" opened error"<<std::endl;
        return;
    }
    string line;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        string word;
        int index;
        iss>>word;
        while(iss>>index)
        {
            _indexTableCn[word].insert(index);
        }
    }
}

vector<std::pair<string,int>> &  Dict_cn::getDictCn()
{
    return _dictCn;
}

unordered_map<string,set<int>> &  Dict_cn::getIndexTableCn()
{
    return _indexTableCn;
}
} //end of namespace keyword_suggestion
