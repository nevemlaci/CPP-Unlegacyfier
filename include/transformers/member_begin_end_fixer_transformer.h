#pragma once

#include <string>
#include <transformer/transformer.h>

class MemberBeginEndFixerTransformer : public Transformer {
  public:
    using Transformer::Transformer;
    void start(clang::ast_matchers::MatchFinder& finder) override;

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

  private:
    void run_impl(
        const clang::ast_matchers::MatchFinder::MatchResult& result,
        const clang::CXXMemberCallExpr* call,
        const clang::Expr* container,
        const std::string& call_name
    );
};