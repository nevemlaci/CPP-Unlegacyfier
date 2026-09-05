#pragma once

#include "transformer/transformer.hpp"

class ReplaceTypedefWithUsingTransformer : public Transformer {
  public:
    using Transformer::Transformer;
    void start(clang::ast_matchers::MatchFinder& finder) override;
    void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;
};