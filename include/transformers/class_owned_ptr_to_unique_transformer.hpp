#pragma once

#include "transformer/transformer.hpp"

class ClassOwnedPointerToUniqueTransformer : public Transformer {
  public:
    using Transformer::Transformer;
    void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;
    void start(clang::ast_matchers::MatchFinder& finder) override;

  private:
};