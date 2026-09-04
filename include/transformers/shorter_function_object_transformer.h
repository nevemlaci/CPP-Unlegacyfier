#pragma once

#include "transformer/transformer.h"

namespace clang {
namespace ast_matchers {
class MatchFinder;
}
} // namespace clang

class ShorterFunctionObjectTransformer : public Transformer {

  public:
    using Transformer::Transformer;

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

    virtual void start(clang::ast_matchers::MatchFinder& finder) override;
};