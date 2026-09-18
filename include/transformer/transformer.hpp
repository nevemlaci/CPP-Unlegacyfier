#pragma once

#include "util/types.hpp"

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/Diagnostic.h>

namespace clang {
class ASTContext;
}

class Transformer : public clang::ast_matchers::MatchFinder::MatchCallback {
  protected:
    clang::ASTContext& context;

  public:
    explicit Transformer(clang::ASTContext& context);
    void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override = 0;
    virtual void start(clang::ast_matchers::MatchFinder& finder) = 0;

    template <std::size_t N>
    clang::DiagnosticBuilder create_diagnostic(
        const char (&format_string)[N], clang::SourceLocation loc,
        clang::DiagnosticsEngine::Level level = clang::DiagnosticsEngine::Level::Warning) {
        auto& diagnositcs_engine = context.getDiagnostics();
        auto diag_id = diagnositcs_engine.getCustomDiagID(level, format_string);
        return diagnositcs_engine.Report(loc, diag_id);
    }
};
