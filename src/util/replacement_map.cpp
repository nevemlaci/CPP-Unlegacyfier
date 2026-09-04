#include "util/replacement_map.hpp"

namespace nl {
ReplacementsMapWrapper::ReplacementsMapWrapper(
    std::map<std::string, clang::tooling::Replacements>& map)
    : m_map(map) {}

void ReplacementsMapWrapper::add_replacement(const clang::tooling::Replacement rep) {
    auto filepath = rep.getFilePath();
    auto err = m_map[filepath.str()].add(rep);

    if (err) {
        llvm::errs() << "Conflicting edit detected in " << filepath << "\n";
    }
}
} // namespace nl
