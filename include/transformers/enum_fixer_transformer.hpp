#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <transformer/transformer.hpp>

class EnumFixerTransformer : public Transformer {
  public:
    using Transformer::Transformer;

    void start(clang::ast_matchers::MatchFinder& finder) override;

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;
};