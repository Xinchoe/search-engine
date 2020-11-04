#include "../../include/offline/configuration.h"
#include "../../include/offline/dict_producer_en.h"
#include "../../include/offline/dict_producer_zh.h"
#include "../../include/offline/split_tool_cppjieba.h"
void Run();

int main() {
  Run();
  return 0;
}

void Run() {
  keyword_suggestion::Configuration path("path.conf");
  keyword_suggestion::DictProducerEn dict_en(path.GetContent("test_corpus_en"));

  keyword_suggestion::SplitTool *sp;
  keyword_suggestion::SplitToolCppJieba stc;
  sp=&stc;
  keyword_suggestion::DictProducer dict_zh(path.GetContent("art"));
  dict_zh.createStopWords(path.GetContent("stop_words_zh"));
  dict_zh.buildCnDict();
  dict_zh.buildCnIndex();
  dict_zh.storeDict(path.GetContent("dict_zh.dat"));
  dict_zh.storeIndex(path.GetContent("index_zh.dat"));
  
  dict_en.BuildStopWordLibEn(path.GetContent("stop_words_en"));
  dict_en.BuildDictEn();
  dict_en.BuildIndexEn();
  dict_en.StoreDictEn(path.GetContent("dict_en"));
  dict_en.StoreIndexEn(path.GetContent("index_en"));
}
