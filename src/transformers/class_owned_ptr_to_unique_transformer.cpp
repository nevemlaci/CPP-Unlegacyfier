#include "transformers/class_owned_ptr_to_unique_transformer.hpp"

#include "clang/AST/Decl.h"
#include "clang/AST/ExprCXX.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Tooling/Core/Replacement.h"

#include <format>

void ClassOwnedPointerToUniqueTransformer::start(clang::ast_matchers::MatchFinder& finder) {
    using namespace clang::ast_matchers;

    // clang-format off
    auto dtorMatcher = cxxDestructorDecl(
        forEachDescendant(
            cxxDeleteExpr(
                has(
                    expr(
                        ignoringParenImpCasts(
                            memberExpr(
                                member(
                                    fieldDecl(
                                        hasType(pointerType()) //
                                    )
                                )
                            )
                        )
                    )
                )
            )
        )
    );
    
    auto initListMatcher = cxxCtorInitializer(
        forField(fieldDecl(
            hasType(pointerType())).bind("field")
        ),
        withInitializer(cxxNewExpr().bind("new_init"))
    ).bind("ctor_init");

    // clang-format on
    finder.addMatcher(dtorMatcher, this);
}

void ClassOwnedPointerToUniqueTransformer::run(
    const clang::ast_matchers::MatchFinder::MatchResult& result) {
    using namespace clang;

    const auto delete_expr = result.Nodes.getNodeAs<CXXDeleteExpr>("delete_expr");
    const auto field = result.Nodes.getNodeAs<FieldDecl>("pointer_field");

    if (!delete_expr || !field) {
        return;
    }

    auto& source_manager = *result.SourceManager;
    auto& lang_opts = result.Context->getLangOpts();

    CharSourceRange delete_range = CharSourceRange::getTokenRange(delete_expr->getSourceRange());
    tooling::Replacement erease_delete_rep(source_manager, delete_range, "");
    shared_replacement_map.add_replacement(erease_delete_rep);

    auto pointee_type = field->getType()->getPointeeType();
    std::string new_type_name = std::format("std::unique_ptr<{}>", pointee_type.getAsString());

    auto type_source_info = field->getTypeSourceInfo();
    if (type_source_info) {
        auto type_range =
            CharSourceRange::getTokenRange(type_source_info->getTypeLoc().getSourceRange());

        tooling::Replacement rewrite_type_rep(source_manager, type_range, new_type_name);
        shared_replacement_map.add_replacement(rewrite_type_rep);
    }
}
