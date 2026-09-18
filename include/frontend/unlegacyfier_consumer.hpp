#pragma once

#include "config/unlegacyfier_config.hpp"
#include "util/types.hpp"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Rewrite/Core/Rewriter.h"

class UnlegacyfierConsumer : public clang::ASTConsumer {
    clang::Rewriter rewriter;
    UnlegacyfierConfig config;

  public:
    explicit UnlegacyfierConsumer(clang::ASTContext& context, UnlegacyfierConfig config);

    void HandleTranslationUnit(clang::ASTContext& context) override;
};