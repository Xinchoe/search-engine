#include "../../include/offline/configuration.h"

namespace keyword_suggestion {

Configuration::Configuration(const std::string &config) {
  FILE *path;
  char prefix[kPathSize] = {0}, index[1024] = {0}, content[1024] = {0};
  std::string dir_path;

  getcwd(prefix, kPathSize);
  for (int i = strlen(prefix) - 1; prefix[i] != '/'; --i) {
    prefix[i] = '\0';
  }
  prefix[strlen(prefix) - 1] = '\0';
  dir_path = static_cast<std::string>(prefix) + "/conf/" + config;

  path = fopen(dir_path.c_str(), "rb");
  while (fscanf(path, "%s%s", index, content) != EOF) {
    config_[static_cast<std::string>(index)] = content;
  }
  fclose(path);
}

std::string Configuration::GetContent(const std::string &config) {
  return config_.find(config)->second;
}

}  // namespace keyword_suggestion
