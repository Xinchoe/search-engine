#ifndef KEYWORD_SUGGESTION_SERVER_INCLUDE_OFFLINE_SPLIT_TOOL_H_
#define KEYWORD_SUGGESTION_SERVER_INCLUDE_OFFLINE_SPLIT_TOOL_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace keyword_suggestion {

class SplitTool {
 public:
  SplitTool() {}
  virtual ~SplitTool() {}

  virtual vector<string> cut(
      const string &sentence) = 0;  //分词函数，纯虚函数提供接口
};

}  // end of namespace keyword_suggestion

#endif
