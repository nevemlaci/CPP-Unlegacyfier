#include "frontend/unlegacyfier_consumer.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"

#include <clang/Frontend/CompilerInstance.h>
#include <frontend/unlegacyfier_frontend.h>

UnlegacyfierFrontend::UnlegacyfierFrontend(UnlegacyfierConfig config,
                                           nl::ReplacementMapRef shared_replacement_map)
    : config(config), shared_replacement_map(shared_replacement_map) {}

std::unique_ptr<clang::ASTConsumer>
UnlegacyfierFrontend::CreateASTConsumer(clang::CompilerInstance& compiler, llvm::StringRef inFile) {
    return std::make_unique<UnlegacyfierConsumer>(compiler.getASTContext(), config,
                                                  shared_replacement_map);
}