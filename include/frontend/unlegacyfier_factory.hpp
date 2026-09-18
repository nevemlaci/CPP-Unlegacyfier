#pragma once

#include "config/unlegacyfier_config.hpp"

#include "clang/Tooling/Tooling.h"

#include <memory>
#include <util/types.hpp>

class UnlegacyfierActionFactory : public clang::tooling::FrontendActionFactory {
    UnlegacyfierConfig config;

  public:
    UnlegacyfierActionFactory(UnlegacyfierConfig config);
    std::unique_ptr<clang::FrontendAction> create() override;
};