#include "frontend/unlegacyfier_factory.hpp"

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Tooling.h"

#include <config/unlegacyfier_config.hpp>
#include <gtest/gtest.h>
#include <transformers/enum_fixer_transformer.hpp>
using namespace clang::ast_matchers;

std::string rewriteCode(const std::string& code, UnlegacyfierConfig config) {
    std::map<std::string, clang::tooling::Replacements> raw_map;
    nl::ReplacementsMapWrapper wrapper(raw_map); //[cite: 2]

    UnlegacyfierActionFactory factory(config, wrapper);

    std::vector<std::string> args = {"-std=c++17", "-fsyntax-only"};
    bool success = clang::tooling::runToolOnCodeWithArgs(factory.create(), code, args, "input.cpp");

    if (!success) {
        return "";
    }

    auto file_replacements = raw_map["input.cpp"];
    auto rewritten = clang::tooling::applyAllReplacements(code, file_replacements);

    if (rewritten) {
        return *rewritten;
    }

    llvm::consumeError(rewritten.takeError());
    return "";
}

TEST(EnumFixerTest, ModifiedEnumIsScoped) {
    UnlegacyfierConfig config;
    config.EnableEnumFixer = true;

    std::string input =
        R"(
enum Color {Red, Green, Blue};
)";

    std::string rewritten = rewriteCode(input, config);

    std::unique_ptr<clang::ASTUnit> AST =
        clang::tooling::buildASTFromCodeWithArgs(rewritten, {"-std=c++17"});
    ASSERT_TRUE(AST != nullptr);

    auto enum_rename_matcher = enumDecl(hasName("Color")).bind("enum");
    auto enum_rename_matches = match(enum_rename_matcher, AST->getASTContext());
    ASSERT_EQ(enum_rename_matches.size(), 1);

    const auto* resultingEnum = enum_rename_matches[0].getNodeAs<clang::EnumDecl>("enum");
    EXPECT_TRUE(resultingEnum->isScoped());
}

TEST(EnumFixerTest, EnumReferencesAreCorrectlyQualified) {
    UnlegacyfierConfig config;
    config.EnableEnumFixer = true;

    std::string input =
        R"(
enum Color {Red, Green, Blue};

int main(){
    Color c = Red;
}
)";

    std::string rewritten = rewriteCode(input, config);

    std::unique_ptr<clang::ASTUnit> AST =
        clang::tooling::buildASTFromCodeWithArgs(rewritten, {"-std=c++17"});
    ASSERT_TRUE(AST != nullptr);
    // clang-format off
    auto matcher = 
        declRefExpr(
            to(enumConstantDecl(
                hasDeclContext(
                    enumDecl(
                        isScoped()
                    )
                )
            ))
        ).bind("enum_ref");

    // clang-format on
    auto matches = match(matcher, AST->getASTContext());
    ASSERT_EQ(matches.size(), 1);
    using namespace clang;
    const auto* const result_enum_ref = matches[0].getNodeAs<DeclRefExpr>("enum_ref");
    ASSERT_TRUE(result_enum_ref->hasQualifier());
    auto qualifier_loc = result_enum_ref->getQualifierLoc();
    std::string qualifier_text =
        clang::Lexer::getSourceText(CharSourceRange::getTokenRange(qualifier_loc.getSourceRange()),
                                    AST->getSourceManager(), AST->getASTContext().getLangOpts())
            .str();

    EXPECT_EQ(qualifier_text, "Color::");
}