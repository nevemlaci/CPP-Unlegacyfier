#pragma once

#include "config/unlegacifier_config.h"
#include "util/types.h"

#include "clang/Frontend/FrontendAction.h"

#include <memory>

namespace clang {
class ASTConsumer;
class CompilerInstance;
} // namespace clang

namespace llvm {
class StringRef;
}

class UnlegacifierFrontend : public clang::ASTFrontendAction {

    UnlegacifierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    UnlegacifierFrontend(UnlegacifierConfig config, nl::ReplacementMapRef shared_replacement_map);

    virtual std::unique_ptr<clang::ASTConsumer>
    CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) override;
};