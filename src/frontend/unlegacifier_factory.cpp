#include "frontend/unlegacyfier_factory.h"
#include "frontend/unlegacyfier_frontend.h"

UnlegacyfierActionFactory::UnlegacyfierActionFactory(UnlegacyfierConfig config,
                                                     nl::ReplacementMapRef shared_replacement_map)
    : config(config), shared_replacement_map(shared_replacement_map) {}

std::unique_ptr<clang::FrontendAction> UnlegacyfierActionFactory::create() {
    return std::make_unique<UnlegacyfierFrontend>(config, shared_replacement_map);
}