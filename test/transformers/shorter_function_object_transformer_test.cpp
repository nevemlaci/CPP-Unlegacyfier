//
// Created by nevemlaci on 2026. 09. 18..
//
#include "config/unlegacyfier_config.hpp"
#include "rewrite_code.hpp"
#include "transformers/enum_fixer_transformer.hpp"

#include "gtest/gtest.h"
using namespace clang::ast_matchers;

TEST(ShorterFunctionObjectTest, IsFunctionObjectShortVersion) {
    UnlegacyfierConfig config;
    config.EnableShorterFunctor = true;

    std::string input =
        R"(
#include <functional>

template <typename F> void foo(const F& f) {}

int main(){
    foo(std::greater<int>());
}
)";

    std::string rewritten = rewrite_code(input, config);

    std::unique_ptr<clang::ASTUnit> AST =
        clang::tooling::buildASTFromCodeWithArgs(rewritten, {"-std=c++17"});
    ASSERT_TRUE(AST != nullptr);

    // clang-format off
    auto matcher =
        callExpr(
            callee(functionDecl(hasName("foo"))),
            hasArgument(0,
                expr(hasType(
                    classTemplateSpecializationDecl(
                        hasName("::std::greater"),
                        hasTemplateArgument(0, refersToType(voidType()))
                    )
                ))
            )
        ).bind("call");
    // clang-format on

    auto matches = match(matcher, AST->getASTContext());
    ASSERT_EQ(matches.size(), 1);
}