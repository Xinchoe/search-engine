#include "../../include/offline/dict_producer_zh.h"
#include "../../include/offline/split_tool_cppjieba.h"
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::cin;
using std::ifstream;
using std::ofstream;

namespace keyword_suggestion
{

DictProducer_zh::DictProducer_zh(const string &dir)
:_dir_zh(dir)
{}

DictProducer_zh::DictProducer_zh(const string &dir,SplitTool *splitTool)
:_dir_zh(dir)
,_splitTool(splitTool)
{
    DIR *pdir=opendir(_dir_zh.c_str());
    struct dirent *pdirent;
    while((pdirent=readdir(pdir)))
    {
        if(strcmp(pdirent->d_name,".")==0||strcmp(pdirent->d_name,"..")==0)
        {
            continue;
        }
        string abpath;
        abpath+=_dir_zh+"/"+pdirent->d_name;
        _files_zh.push_back(abpath);
        /* if(strcmp(pdirent->d_name,"stop_words_zh.txt")) */
        /* { */ 
        /*     ifstream in(abpath); */
        /*     string word; */
        /*     while(getline(in,word)) */
        /*     { */
        /*         ++_stopWords_zh[word]; */
        /*     } */
        /* } */
    }
}

void DictProducer_zh::createStopWords(const string &filepath)
{
    ifstream in(filepath);
    string word;
    while(getline(in,word))
    {
        /* cout<<word<<endl; */
        _stopWords_zh[word]=1;
    }
    /* auto it=_stopWords_zh.begin(); */
    /* cout<<it->first<<" "<<it->second<<endl; */
}

void DictProducer_zh::buildCnDict()
{
    for(auto &i:_files_zh)
    {
        /* cout<<_files_zh.size()<<endl; */
        /* cout<<i<<endl; */
        ifstream in(i);
        string text;
        string line;
        vector<string> words;
        while(getline(in,line))
        {
            /* cout<<line<<endl; */
            text+=line;
            /* cout<<text<<endl; */
        }
        words=_splitTool->cut(text);
        for(auto &i:words)
        {
            //停用词不加入字典
            if(_stopWords_zh.find(i)!=_stopWords_zh.end())
            {
                /* cout<<_stopWords_zh[i]<<endl; */
                continue;
            }
            auto it=_dict_zh.begin();
            while(it!=_dict_zh.end())
            {
                if(it->first==i)
                {
                    it->second+=1;
                    break;
                }
                else
                {
                    ++it;
                }
            }
            if(it==_dict_zh.end())
            {
                _dict_zh.push_back(make_pair(i,1));
            }
        }
    }
}

void DictProducer_zh::storeDict(const string &filepath)
{
    ofstream out(filepath);
    for(auto &i:_dict_zh)
    {
        /* cout<<i.first<<": "<<i.second<<endl; */
        out<<i.first<<" "<<i.second<<endl;
    }
}

void DictProducer_zh::showFiles()const
{
    cout<<_dir_zh<<endl;
}

void DictProducer_zh::showDict()const
{
    for(auto &i:_dict_zh)
    {
        cout<<i.first<<": "<<i.second<<endl;
    }
}

void DictProducer_zh::getFiles()
{
    DIR *pdir=opendir(_dir_zh.c_str());
    struct dirent *pdirent;
    while((pdirent=readdir(pdir)))
    {
        if(strcmp(pdirent->d_name,".")==0||strcmp(pdirent->d_name,"..")==0)
        {
            continue;
        }
        cout<<pdirent->d_name<<endl;   
    }
}
//创建索引，遍历字典中的每一个中文字节
void DictProducer_zh::buildCnIndex()
{
    for(size_t i=0;i<_dict_zh.size();++i)
    {
        for(size_t j=0;j<=_dict_zh[i].first.size();j+=3)
        {   
            string tmp;
            tmp=_dict_zh[i].first.substr(j,3);
            _index_zh[tmp].insert(i);
        }
    }
}

void DictProducer_zh::storeIndex(const string &filepath)
{
    /* cout<<"storeIndex()"<<endl; */
    ofstream out(filepath);

    for(auto &i:_index_zh)
    {
        out<<i.first<<" ";
        for(auto &j:i.second)
        {
            out<<j<<" ";
        }
        out<<endl;
    }
}

}//end of namespace keyword_suggestion
