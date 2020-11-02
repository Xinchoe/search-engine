#ifndef INCLUDE_DICT_PRODUCER_EN_H_
#define INCLUDE_DICT_PRODUCER_EN_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace keyword_suggestion {

class SplitTool;

class DictProducerEn {
 public:
  DictProducerEn(const std::string &dir);

  void BuildDictEn();
  void PushDictEn(const std::string &word);
  void StoreDictEn(const char *file_path);
  void ShowDictEn() const;
  void ShowFilesPathEn() const;
  void GetFilesPathEn();

 private:
  std::string dir_en_;
  std::vector<std::string> files_en_;
  std::map<std::string, int> dict_en_;
  SplitTool *split_tool_en_;
};

}  // namespace keyword_suggestion

#endif
