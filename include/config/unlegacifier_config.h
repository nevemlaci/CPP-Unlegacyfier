#pragma once

struct UnlegacifierConfig {
    bool EnableEnumFixer = false;
    bool EnableBeginEndFixer = false;
    bool EnableShorterFunctor = false;
    bool EnableAddMissingOverride = false;
    bool EnableReplaceTypedefWithUsing = false;
};