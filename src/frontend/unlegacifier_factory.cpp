#include "frontend/unlegacyfier_factory.hpp"
#include "frontend/unlegacyfier_frontend.hpp"

UnlegacyfierActionFactory::UnlegacyfierActionFactory(UnlegacyfierConfig config) : config(config) {}

std::unique_ptr<clang::FrontendAction> UnlegacyfierActionFactory::create() {
    return std::make_unique<UnlegacyfierFrontend>(config);
}