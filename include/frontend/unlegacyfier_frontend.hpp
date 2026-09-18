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

  public:
    UnlegacyfierFrontend(UnlegacyfierConfig config);

  protected:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler,
                                                          llvm::StringRef inFile) override;
};