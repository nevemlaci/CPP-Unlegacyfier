#include "transformers/member_begin_end_fixer_transformer.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Core/Replacement.h"

void MemberBeginEndFixerTransformer::start(clang::ast_matchers::MatchFinder& finder) {
    using namespace clang::ast_matchers;

    auto beginCallMathcer = cxxMemberCallExpr(callee(cxxMethodDecl(hasName("begin"))),
                                              on(expr().bind("begin_container_expr")),
                                              unless(isExpansionInSystemHeader()))
                                .bind("begin_call");

    auto endCallMatcher = cxxMemberCallExpr(callee(cxxMethodDecl(hasName("end"))),
                                            on(expr().bind("end_container_expr")),
                                            unless(isExpansionInSystemHeader()))
                              .bind("end_call");
    finder.addMatcher(beginCallMathcer, this);
    finder.addMatcher(endCallMatcher, this);
}

void MemberBeginEndFixerTransformer::run(
    const clang::ast_matchers::MatchFinder::MatchResult& result) {
    using namespace clang;

    const auto* const beginCall = result.Nodes.getNodeAs<CXXMemberCallExpr>("begin_call");
    const auto* const beginContainer = result.Nodes.getNodeAs<Expr>("begin_container_expr");

    if ((beginCall != nullptr) && (beginContainer != nullptr)) {
        run_impl(result, beginCall, beginContainer, "std::begin");
    }
}

void MemberBeginEndFixerTransformer::run_impl(
    const clang::ast_matchers::MatchFinder::MatchResult& result,
    const clang::CXXMemberCallExpr* call, const clang::Expr* container,
    const std::string& call_name) {
    auto* const source_manager = result.SourceManager;
    const auto lang_options = result.Context->getLangOpts();

    auto container_range = clang::CharSourceRange::getTokenRange(container->getSourceRange());
    auto container_text =
        clang::Lexer::getSourceText(container_range, *source_manager, lang_options);

    if (container_text.empty()) {
        return;
    }

    bool is_arrow = false;

    if (const auto* const member_expr = llvm::dyn_cast<clang::MemberExpr>(call->getCallee())) {
        is_arrow = member_expr->isArrow();
    }

    std::string new_text;
    if (is_arrow) {
        new_text = call_name + "(*" + container_text.str() + ")";
    } else {
        new_text = call_name + "(" + container_text.str() + ")";
    }

    auto call_range = clang::CharSourceRange::getTokenRange(call->getSourceRange());

    clang::tooling::Replacement rep(*source_manager, call_range, new_text, lang_options);
    shared_replacement_map.add_replacement(rep);
}
