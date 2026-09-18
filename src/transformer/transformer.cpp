#include "transformer/transformer.hpp"

#include "clang/Rewrite/Core/Rewriter.h"

Transformer::Transformer(clang::ASTContext& context) : context(context) {}