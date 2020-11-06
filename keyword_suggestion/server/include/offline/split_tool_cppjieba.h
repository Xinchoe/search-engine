#ifndef KEYWORD_SUGGESTION_SERVER_INCLUDE_OFFLINE_SPLIT_TOOL_CPPJIEBA_H_
#define KEYWORD_SUGGESTION_SERVER_INCLUDE_OFFLINE_SPLIT_TOOL_CPPJIEBA_H_

#include "split_tool.h"

namespace keyword_suggestion {

class SplitToolCppJieba : public SplitTool {
 private:
  bool isChinese(const string &);

 public:
  vector<string> cut(const string &sentence) override;
};

}  // end of namespace keyword_suggestion

#endif
