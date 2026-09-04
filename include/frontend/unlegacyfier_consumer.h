#pragma once

#include "config/unlegacyfier_config.h"
#include "util/types.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Rewrite/Core/Rewriter.h"

class UnlegacyfierConsumer : public clang::ASTConsumer {
    clang::Rewriter rewriter;
    UnlegacyfierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    explicit UnlegacyfierConsumer(clang::ASTContext& context, UnlegacyfierConfig config,
                                  nl::ReplacementMapRef shared_replacement_map);

    void HandleTranslationUnit(clang::ASTContext& context) override;
};