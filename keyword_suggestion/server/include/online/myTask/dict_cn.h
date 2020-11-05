#ifndef _MYDICT_H_
#define _MYDICT_H_

#include <iostream>
#include <unordered_map>
#include <string>
#include <set>
#include <vector>

using std::unordered_map;
using std::set;
using std::string;
using std::vector;
namespace keyword_suggestion
{

class Dict_cn
{
public:
    static Dict_cn * createInstance();

    void initDictCn(const string & dictCnPath);
    void initIndexTableCn(const string & indexTableCnPath);

    vector<std::pair<string,int>> &  getDictCn();

    unordered_map<string,set<int>> & getIndexTableCn();
private:
    class AutoRelease
    {
    public:
        AutoRelease() {}
        ~AutoRelease()
        {
            /* std::cout<<"~AutoRelease()"<<std::endl; */
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance=nullptr;
            }
        }
    };
    Dict_cn() {}
    ~Dict_cn() {}
private:
    static Dict_cn *_pInstance;
    vector<std::pair<string,int>> _dictCn; //词典
    unordered_map<string,set<int>> _indexTableCn;  //索引表
    
    static  AutoRelease _auto;

};
} //end of namespace keyword_suggestion
#endif
