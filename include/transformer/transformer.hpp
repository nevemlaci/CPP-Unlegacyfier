#pragma once

#include "util/types.hpp"

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticIDs.h"

#include <clang/ASTMatchers/ASTMatchFinder.h>

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

    clang::DiagnosticBuilder
    create_diagnostic(const std::string& desc, clang::SourceLocation loc,
                      clang::diag::Severity severity = clang::diag::Severity::Warning);
};
