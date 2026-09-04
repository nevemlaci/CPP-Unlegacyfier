#pragma once

#include "util/types.hpp"

#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace clang {
class ASTContext;
}

class Transformer : public clang::ast_matchers::MatchFinder::MatchCallback {
  protected:
    clang::ASTContext& context;
    clang::Rewriter& rewriter;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    explicit Transformer(
        clang::ASTContext& context,
        clang::Rewriter& rewriter,
        nl::ReplacementMapRef shared_replacement_map
    );
    void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override = 0;
    virtual void start(clang::ast_matchers::MatchFinder& finder) = 0;
};
