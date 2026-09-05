#include "transformers/replace_typedef_with_using_transformer.hpp"

#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Core/Replacement.h"

void ReplaceTypedefWithUsingTransformer::start(clang::ast_matchers::MatchFinder& finder) {
    using namespace clang::ast_matchers;
    // clang-format off
    auto matcher = typedefDecl(
        unless(isExpansionInSystemHeader())
    ).bind("typedef");

    // clang-format on
    finder.addMatcher(matcher, this);
}

void ReplaceTypedefWithUsingTransformer::run(
    const clang::ast_matchers::MatchFinder::MatchResult& result) {
    using namespace clang;
    const auto* const typedef_decl = result.Nodes.getNodeAs<TypedefDecl>("typedef");
    if (typedef_decl == nullptr) {
        return;
    }

    auto* tag = typedef_decl->getUnderlyingType()->getAsTagDecl();

    if ((tag != nullptr) && (tag->getIdentifier() == nullptr) && tag->isEmbeddedInDeclarator()) {
        // unnamed typedeffed struct/enum
        return;
    }

    auto printing_policy = result.Context->getPrintingPolicy();
    printing_policy.SuppressTagKeyword = true;

    auto begin_location = typedef_decl->getBeginLoc();

    if (begin_location.isInvalid() || begin_location.isMacroID() ||
        !result.SourceManager->isInMainFile(begin_location)) {
        return;
    }

    std::string alias_name = typedef_decl->getNameAsString();

    auto type_range = typedef_decl->getTypeSourceInfo()->getTypeLoc().getSourceRange();

    std::string right_side = typedef_decl->getUnderlyingType().getAsString(printing_policy);

    if (right_side.empty()) {
        return;
    }

    std::string replacement_text = std::format("using {} = {}", alias_name, right_side);

    auto replace_range = CharSourceRange::getTokenRange(typedef_decl->getSourceRange());

    tooling::Replacement rep(*result.SourceManager, replace_range, replacement_text);

    shared_replacement_map.add_replacement(rep);
}