#pragma once

#include "util/types.hpp"

#include <clang/Tooling/ReplacementsYaml.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/YAMLTraits.h>
class YamlFrontend {
    nl::ReplacementMapRef replacement_map;

  public:
    YamlFrontend(nl::ReplacementMapRef replacement_map);

    void export_to_yaml(const std::string& output_filename);
};