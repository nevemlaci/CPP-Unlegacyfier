#include "frontend/unlegacyfier_consumer.hpp"
#include "transformers/add_missing_override_transformer.hpp"
#include "transformers/enum_fixer_transformer.hpp"
#include "transformers/member_begin_end_fixer_transformer.hpp"
#include "transformers/replace_typedef_with_using_transformer.hpp"
#include "transformers/shorter_function_object_transformer.hpp"

UnlegacyfierConsumer::UnlegacyfierConsumer(clang::ASTContext& /*context*/,
                                           UnlegacyfierConfig config,
                                           nl::ReplacementMapRef shared_replacement_map)
    : config(config), shared_replacement_map(shared_replacement_map) {}

void UnlegacyfierConsumer::HandleTranslationUnit(clang::ASTContext& context) {
    rewriter.setSourceMgr(context.getSourceManager(), context.getLangOpts());

    EnumFixerTransformer enum_fixer(context, rewriter, shared_replacement_map);
    MemberBeginEndFixerTransformer beginEndFixer(context, rewriter, shared_replacement_map);
    ShorterFunctionObjectTransformer shorterFunctorFixer(context, rewriter, shared_replacement_map);
    AddMissingOverrideTransformer addMissingOverrideTransformer(context, rewriter,
                                                                shared_replacement_map);
    ReplaceTypedefWithUsingTransformer replaceTypedefWithUsingTransformer(context, rewriter,
                                                                          shared_replacement_map);

    clang::ast_matchers::MatchFinder finder;
    if (config.EnableEnumFixer) {
        enum_fixer.start(finder);
    }
    if (config.EnableBeginEndFixer) {
        beginEndFixer.start(finder);
    }
    if (config.EnableShorterFunctor) {
        shorterFunctorFixer.start(finder);
    }
    if (config.EnableAddMissingOverride) {
        addMissingOverrideTransformer.start(finder);
    }
    if (config.EnableReplaceTypedefWithUsing) {
        replaceTypedefWithUsingTransformer.start(finder);
    }

    finder.matchAST(context);

    const auto* buffer = rewriter.getRewriteBufferFor(context.getSourceManager().getMainFileID());

    if (buffer != nullptr) {
        buffer->write(llvm::outs());
    }
}