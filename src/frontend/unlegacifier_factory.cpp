#include "frontend/unlegacyfier_factory.hpp"
#include "frontend/unlegacyfier_frontend.hpp"

UnlegacyfierActionFactory::UnlegacyfierActionFactory(UnlegacyfierConfig config,
                                                     nl::ReplacementMapRef shared_replacement_map)
    : config(config), shared_replacement_map(shared_replacement_map) {}

std::unique_ptr<clang::FrontendAction> UnlegacyfierActionFactory::create() {
    return std::make_unique<UnlegacyfierFrontend>(config, shared_replacement_map);
}