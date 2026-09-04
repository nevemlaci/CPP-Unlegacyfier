#pragma once

#include "transformer/transformer.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"

class AddMissingOverrideTransformer : public Transformer {
    using Transformer::Transformer;

  public:
    void start(clang::ast_matchers::MatchFinder& finder) override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

  private:
    clang::SourceLocation find_last_equals_sign_in_method_decl(
        const clang::ast_matchers::MatchFinder::MatchResult& result,
        const clang::CXXMethodDecl* const method
    );
};