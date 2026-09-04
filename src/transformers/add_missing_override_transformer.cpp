#include "transformers/add_missing_override_transformer.h"

#include "clang/Lex/Lexer.h"

void AddMissingOverrideTransformer::start(clang::ast_matchers::MatchFinder& finder) {
    using namespace clang::ast_matchers;
    auto matcher =
        cxxMethodDecl(isOverride(), unless(isExpansionInSystemHeader())).bind("overriding_method");

    finder.addMatcher(matcher, this);
}

void AddMissingOverrideTransformer::run(
    const clang::ast_matchers::MatchFinder::MatchResult& result) {
    using namespace clang;
    const auto method = result.Nodes.getNodeAs<CXXMethodDecl>("overriding_method");

    if (!method || method->hasAttr<OverrideAttr>() || method->hasAttr<FinalAttr>()) {
        return;
    }

    if (method->isImplicit() || result.SourceManager->isInSystemHeader(method->getLocation())) {
        return;
    }

    SourceLocation insert_location;

    if (method->hasBody()) {
        insert_location = method->getBody()->getBeginLoc();
    } else if (method->isPureVirtual()) {
        auto last_eq_loc = find_last_equals_sign_in_method_decl(result, method);
        if (last_eq_loc.isValid()) {
            insert_location = last_eq_loc;
        } else {
            return;
        }
    } else {
        insert_location = method->getEndLoc();
    }

    if (insert_location.isValid()) {
        tooling::Replacement rep(*result.SourceManager, insert_location, 0, " override ");

        shared_replacement_map.add_replacement(rep);
    }
}

clang::SourceLocation AddMissingOverrideTransformer::find_last_equals_sign_in_method_decl(
    const clang::ast_matchers::MatchFinder::MatchResult& result,
    const clang::CXXMethodDecl* const method) {
    const auto& source_manager = *result.SourceManager;
    const auto& lang_options = result.Context->getLangOpts();

    auto current_location = method->getBeginLoc();

    clang::SourceLocation last_equal_loc;

    while (current_location.isValid()) {
        std::optional<clang::Token> token =
            clang::Lexer::findNextToken(current_location, source_manager, lang_options);
        if (!token) {
            break;
        }

        if (token->is(clang::tok::equal)) {
            last_equal_loc = token->getLocation();
        }

        if (token->is(clang::tok::semi) || token->is(clang::tok::l_brace) ||
            token->is(clang::tok::eof)) {
            break;
        }

        current_location = token->getLocation();

        if (source_manager.isBeforeInTranslationUnit(method->getEndLoc(), current_location) ||
            current_location == method->getEndLoc()) {
            break;
        }
    }

    return last_equal_loc;
}