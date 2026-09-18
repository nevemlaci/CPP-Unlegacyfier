#include "frontend/unlegacyfier_consumer.hpp"

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"

#include <clang/Frontend/CompilerInstance.h>
#include <frontend/unlegacyfier_frontend.hpp>

UnlegacyfierFrontend::UnlegacyfierFrontend(UnlegacyfierConfig config) : config(config) {}

std::unique_ptr<clang::ASTConsumer>
UnlegacyfierFrontend::CreateASTConsumer(clang::CompilerInstance& compiler,
                                        llvm::StringRef /*inFile*/) {
    return std::make_unique<UnlegacyfierConsumer>(compiler.getASTContext(), config);
}