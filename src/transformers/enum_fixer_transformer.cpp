#include "transformers/enum_fixer_transformer.hpp"

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Core/Replacement.h"

void EnumFixerTransformer::start(clang::ast_matchers::MatchFinder& finder) {
    using namespace clang::ast_matchers;

    auto unscopedEnumMatcher =
        enumDecl(unless(isScoped()), unless(isExpansionInSystemHeader())).bind("old_enum");

    auto enumRefMatcher =
        declRefExpr(
            to(enumConstantDecl(hasDeclContext(enumDecl(unless(isScoped())).bind("enum_decl")))
                   .bind("enum_const")),
            unless(isExpansionInSystemHeader()))
            .bind("enum_ref");
    finder.addMatcher(unscopedEnumMatcher, this);
    finder.addMatcher(enumRefMatcher, this);
}

void EnumFixerTransformer::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    using namespace clang;

    if (const EnumDecl* enumDecl = result.Nodes.getNodeAs<EnumDecl>("old_enum")) {
        if (result.SourceManager->isInSystemHeader(enumDecl->getSourceRange().getBegin())) {
            return;
        }

        tooling::Replacement rep(*result.SourceManager, enumDecl->getLocation(), 0, "class ");

        shared_replacement_map.add_replacement(rep);
    }

    if (const auto refExpr = result.Nodes.getNodeAs<DeclRefExpr>("enum_ref")) {
        const auto enum_d = result.Nodes.getNodeAs<EnumDecl>("enum_decl");

        if (!enum_d || enum_d->getName().empty()) {
            return;
        }

        if (refExpr->hasQualifier()) {
            return;
        }

        std::string qualifier = enum_d->getNameAsString() + "::";

        tooling::Replacement rep(*result.SourceManager, refExpr->getBeginLoc(), 0, qualifier);

        shared_replacement_map.add_replacement(rep);
    }
}