#pragma once

#include "clang/Tooling/Core/Replacement.h"

#include <map>
#include <string>

namespace nl {
class ReplacementsMapWrapper {
  private:
    std::map<std::string, clang::tooling::Replacements>& m_map;

  public:
    using iterator = std::map<std::string, clang::tooling::Replacements>::iterator;
    explicit ReplacementsMapWrapper(std::map<std::string, clang::tooling::Replacements>& map);

    void add_replacement(const clang::tooling::Replacement rep);

    iterator begin() { return m_map.begin(); }

    iterator end() { return m_map.end(); }
};
} // namespace nl