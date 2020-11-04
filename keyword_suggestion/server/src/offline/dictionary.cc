#include "../../include/offline/configuration.h"
#include "../../include/offline/dict_producer_en.h"

void Run();

int main() {
  Run();
  return 0;
}

void Run() {
  keyword_suggestion::Configuration path("path.conf");
  keyword_suggestion::DictProducerEn dict_en(path.GetContent("test_corpus_en"));

  dict_en.BuildStopWordLibEn(path.GetContent("stop_words_en"));
  dict_en.BuildDictEn();
  dict_en.BuildIndexEn();
  dict_en.StoreDictEn(path.GetContent("dict_en"));
  dict_en.StoreIndexEn(path.GetContent("index_en"));
}
