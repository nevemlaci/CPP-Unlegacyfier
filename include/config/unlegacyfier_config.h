#pragma once

struct UnlegacyfierConfig {
    bool EnableEnumFixer = false;
    bool EnableBeginEndFixer = false;
    bool EnableShorterFunctor = false;
    bool EnableAddMissingOverride = false;
    bool EnableReplaceTypedefWithUsing = false;
};