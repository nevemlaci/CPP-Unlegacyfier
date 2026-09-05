#include "transformers/shorter_function_object_transformer.hpp"

#include "clang/AST/ExprCXX.h"
#include "clang/AST/TypeLoc.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Tooling/Core/Replacement.h"

void ShorterFunctionObjectTransformer::run(
    const clang::ast_matchers::MatchFinder::MatchResult& result) {
    using namespace clang;

    const auto* const expr = result.Nodes.getNodeAs<CXXTemporaryObjectExpr>("functor_expr");
    if (expr == nullptr) {
        return;
    }

    auto* type_source_info = expr->getTypeSourceInfo();

    if (type_source_info == nullptr) {
        return;
    }

    auto type_loc = type_source_info->getTypeLoc().IgnoreParens();

    if (auto elaborated = type_loc.getAs<ElaboratedTypeLoc>()) {
        type_loc = elaborated.getNamedTypeLoc();
    }

    if (auto spec_loc = type_loc.getAs<TemplateSpecializationTypeLoc>()) {
        auto left_angled = spec_loc.getLAngleLoc();
        auto right_angled = spec_loc.getRAngleLoc();

        if (left_angled.isValid()) {
            auto sstart = left_angled.getLocWithOffset(1);
            auto range = CharSourceRange::getCharRange(sstart, right_angled);

            tooling::Replacement rep(*result.SourceManager, range, "");
            shared_replacement_map.add_replacement(rep);
        }
    }
}

void ShorterFunctionObjectTransformer::start(clang::ast_matchers::MatchFinder& finder) {
    using namespace clang::ast_matchers;
    // clang-format off
    auto matcher =
        cxxTemporaryObjectExpr(
            unless(isExpansionInSystemHeader()),
            hasDeclaration(
                cxxMethodDecl(
                    ofClass(
                        classTemplateSpecializationDecl(
                            hasAnyName("::std::greater", "::std::less", "::std::greater_equal",
                           "::std::less_equal", "::std::equal_to", "::std::not_equal_to",
                           "::std::plus", "::std::minus", "::std::multiplies", "::std::divides",
                           "::std::modulus", "::std::bit_and", "::std::bit_or", "::std::bit_xor",
                           "::std::logical_and", "::std::logical_or", "::std::logical_not"
                        ),
                            unless(hasTemplateArgument(0, refersToType(voidType())))
                        )
                    )
                )
            )
        ).bind("functor_expr");
    // clang-format on
    finder.addMatcher(matcher, this);
}