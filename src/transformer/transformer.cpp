#include "transformer/transformer.h"

#include "clang/Rewrite/Core/Rewriter.h"
Transformer::Transformer(
    clang::ASTContext& context,
    clang::Rewriter& rewriter,
    nl::ReplacementMapRef shared_replacement_map
)
    : context(context), rewriter(rewriter), shared_replacement_map(shared_replacement_map) {}