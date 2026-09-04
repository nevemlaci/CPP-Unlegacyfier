#include "frontend/unlegacifier_factory.h"

#include "frontend/unlegacifier_frontend.h"
UnlegacifierActionFactory::UnlegacifierActionFactory(
    UnlegacifierConfig config, nl::ReplacementMapRef shared_replacement_map
)
    : config(config), shared_replacement_map(shared_replacement_map) {}

std::unique_ptr<clang::FrontendAction> UnlegacifierActionFactory::create() {
    return std::make_unique<UnlegacifierFrontend>(config, shared_replacement_map);
}