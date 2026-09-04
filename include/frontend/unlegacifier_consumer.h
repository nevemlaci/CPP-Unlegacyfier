#pragma once

#include "config/unlegacifier_config.h"
#include "util/types.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Rewrite/Core/Rewriter.h"

class UnlegacifierConsumer : public clang::ASTConsumer {
    clang::Rewriter rewriter;
    UnlegacifierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    explicit UnlegacifierConsumer(
        clang::ASTContext& context,
        UnlegacifierConfig config,
        nl::ReplacementMapRef shared_replacement_map
    );

    void HandleTranslationUnit(clang::ASTContext& context) override;
};