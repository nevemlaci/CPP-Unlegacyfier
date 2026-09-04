#pragma once

#include "config/unlegacyfier_config.h"

#include "clang/Tooling/Tooling.h"

#include <memory>
#include <util/types.h>

class UnlegacyfierActionFactory : public clang::tooling::FrontendActionFactory {
    UnlegacyfierConfig config;
    nl::ReplacementMapRef shared_replacement_map;

  public:
    UnlegacyfierActionFactory(UnlegacyfierConfig config,
                              nl::ReplacementMapRef shared_replacement_map);
    std::unique_ptr<clang::FrontendAction> create() override;
};