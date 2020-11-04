#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include "split_tool.h"
namespace keyword_suggestion
{

class SplitToolCppJieba
:public SplitTool
{
private:
    bool isChinese(const string &);
public:
    vector<string> cut(const string &sentence)override;
};
}//end of namespace keyword_suggestion
#endif

