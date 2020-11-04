#ifndef INCLUDE_CONFIGURATION_H_
#define INCLUDE_CONFIGURATION_H_

#include <unistd.h>

#include <string>
#include <unordered_map>

namespace keyword_suggestion {

class Configuration {
  const int kPathSize = 1024;

 public:
  Configuration(const std::string &config);

  std::string GetContent(const std::string &config);

 private:
  std::unordered_map<std::string, std::string> config_;
};

}  // namespace keyword_suggestion

#endif
