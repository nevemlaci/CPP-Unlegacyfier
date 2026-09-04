#pragma once

#include "config/unlegacyfier_config.hpp"
#include "util/types.hpp"

#include "clang/Frontend/FrontendAction.h"

#include <memory>

namespace clang {
class ASTConsumer;
class CompilerInstance;
} // namespace clang

namespace llvm {
class StringRef;
}

class UnlegacyfierFrontend : public clang::ASTFrontendAction {

    UnlegacyfierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    UnlegacyfierFrontend(UnlegacyfierConfig config, nl::ReplacementMapRef shared_replacement_map);

    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler,
                                                                  llvm::StringRef inFile) override;
};